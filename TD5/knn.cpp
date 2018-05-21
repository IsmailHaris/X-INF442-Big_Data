#include "knn.h"
#include "mpi.h"

#include <cstdio>
#include <limits>
#include <iostream>

using namespace std;

int Knn::Classify(Mail* mail_to_classify) const {
    int* labels = new int[k_];
    int* distances = new int[k_];
    this->InitNearestNeighbours(labels, distances);

    this->NearestNeighbours(mail_to_classify, labels, distances);

    int label = PredictedLabel(labels);

    // Frees the allocated memory
    delete[] labels;
    delete[] distances;

    return label;
}

void Knn::Classify(Mail* mail_to_classify, double* thresholds, int* predicted_labels, int num_predictions) const {
    int* labels = new int[k_];
    int* distances = new int[k_];
    this->InitNearestNeighbours(labels, distances);

    this->NearestNeighbours(mail_to_classify, labels, distances);

    for (int i = 0; i < num_predictions; i++) {
        predicted_labels[i] = PredictedLabel(labels, thresholds[i]);
    }

    // Frees the allocated memory
    delete[] labels;
    delete[] distances;
}

void Knn::InitNearestNeighbours(int* labels, int* distances) const {
    // Arrays to store the k nearest neighbours
    for (int i = 0; i < k_; ++i) {
        labels[i] = -1;
        distances[i] = std::numeric_limits<int>::max();
    }
}




void Knn::Merge(int* local_labels, int* local_distances, const int* other_labels, const int* other_distances) const{
    int cur_local = k_;
    int cur_other = k_;
    // Gets rid of the neighbours which are too far away
    while (cur_local + cur_other > k_) {
        if (local_distances[cur_local-1] > other_distances[cur_other-1]) {
            cur_local--;
        } else {
            cur_other--;
        }
    }
    // Merges the two arrays in the local one
    while (cur_local || cur_other ) {
        if (!cur_local || (cur_other &&
                    local_distances[cur_local-1] <= other_distances[cur_other-1])) {
            cur_other--;
            local_distances[cur_local + cur_other] = other_distances[cur_other];
            local_labels[cur_local + cur_other] = other_labels[cur_other];
        } else {
            cur_local--;
            local_distances[cur_local + cur_other] = local_distances[cur_local];
            local_labels[cur_local + cur_other] = local_labels[cur_local];
        }
    }
}


int Knn::PredictedLabel(int* labels) const {
    int num_spam = 0;
    int num_ham = 0;
    for (int i = 0; i < k_; ++i) {
        switch(labels[i]) {
            case 0:
                num_ham++;
                break;
            case 1:
                num_spam++;
                break;
            default:
                break;
        }
    }
    return num_spam > num_ham ? 1 : 0;
}

void Knn::NearestNeighbours(Mail* mail_to_classify, int* labels, int* distances) const {
  InitNearestNeighbours(labels, distances);

  vector<Mail*> train_mails = train_instances_->instances();

  for (vector<Mail*>::iterator it = train_mails.begin(); it != train_mails.end(); it++){
    Insert(labels, distances, mail_to_classify, *it);
  }
}

int Knn::ClassifyMpi(Mail* mail_to_classify) const {

    const int root = 0;
    int numtasks, taskid;

    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    vector<Mail*> train_mails = train_instances_->instances();
    int size = train_mails.size();
    int size_local;

    int label = 0;
    int* labels_local = new int[k_];
    int* distances_local = new int[k_];
    this->InitNearestNeighbours(labels_local, distances_local);

    // Division euclidienne pour répartir la charge de chaque processeur
    int q = size/numtasks;
    int r = size%numtasks;

    // On réparti notamment le reste sur les r premiers processeurs
    if(taskid < r){
        size_local = q+1;
    }else{
        size_local = q;
    }

    /* En fait on peut pas utiliser la fonction NearestNeighbours directement,
    ** puisqu'on ne veut pas l'appliquer à l'intégralité de train_mails
    ** mais seulement à des portions (selon les processeurs)
    ** donc on en fait une copie 
    */
    for (int i=q*taskid; i<q*taskid+size_local; i++){
        Insert(labels_local, distances_local, mail_to_classify, train_mails[i]);
    }

    // Communication blocante : le root doit attendre l'exécution des autres processeurs
    if(taskid != root){ // Autres processeurs
        MPI_Send(labels_local, k_, MPI_INT, root, 0, MPI_COMM_WORLD);
        MPI_Send(distances_local, k_, MPI_INT, root, 1, MPI_COMM_WORLD);
    }else{ // Root
        int* labels_recv = new int[k_];
        int* distances_recv = new int[k_];
        MPI_Status status;
        for(int p=1; p<numtasks; p++){
            MPI_Recv(labels_recv, k_, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(distances_recv, k_, MPI_INT, p, 1, MPI_COMM_WORLD, &status);
            this->Merge(labels_local, distances_local, labels_recv, distances_recv); 
        }
        label = PredictedLabel(labels_local);

        // Frees the allocated memory in root
        delete[] labels_recv;
        delete[] distances_recv;
    }

    // Frees the allocated memory
    delete[] labels_local;
    delete[] distances_local;

    return label;
}

int Knn::PredictedLabel(int* labels, double threshold) const {
  double s = 0;
  for(int i=0; i<k_; i++){
    if (labels[i]){ s++; }
  }

  return (double(s)/double(k_) >= threshold);
}

void Knn::Insert(int* labels, int* distances, Mail* mail_to_classify, Mail* train_mail) const {
  int dist = mail_to_classify->Distance(train_mail);
  int first = distances[0];
  int last = distances[k_-1];

  if (dist < last) {
    int i = k_-1;  
    while ((i > 0) && (distances[i-1] > dist)){   
       distances[i] = distances[i-1];
       labels[i] = labels[i-1];
       i = i - 1;
    }
    distances[i] = dist; 
    labels[i] = train_mail->label();
  }

}

