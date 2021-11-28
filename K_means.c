#include <stdio.h>


// define vertex struct
typedef struct vertex
{
    double x[12]; // we have to give a size to the array ------------------------------------------------------------------
    int mu;
};



// create all_x_array
// input: filename
// output: read file and update  all_x_array -  an array of vertex , initial all mus to null
void initial_all_x_array( struct vertex all_x_array[12], FILE fp){


}


// create mu_array
// input: all_x_array
// outpit: mu_array - the firt K vertex, dont forget to update thier mus
void initialze_mus_array( struct vertex mus_array[12]){

}



// argmin
// input: vertex, mu_array
// outpot: the index of its clostest mu
int argmin(struct vertex xi[12], double mus_array[12] ){

}

// update_mu_for_all_vertex
// input: all_x_array
// output: for evrey vertex in all_x_array update mu according to argmin
void update_mu_for_all_vertex(struct vertex all_x_array[12]){

}


//add_vetex_to_mu
//input: vertex, new_mu_array
// output: add x of vertex to new_mu_array by cords
void add_x_to_mu(struct vertex xi, double new_mus_array[12]){

}


// divide_all_cord_in_mus_array
// input: new_mu_array, counter of vertex in mu
// output: divide all cord of new_mu_array by the counter
void divide_all_cords_in_new_mus_array(int cnt, double new_mus_array[12]){

}


// update _mus
// input: mus_array, all_x_vertex
// output: compute and return new_mus_array 
void update_new_mu(double new_mus_array[12][12]){

}


// euqlide_norm
// input: old_mu, new_mu
// output: (old_mu**2 -new_mu**2)**0.5
double euqlide_norm(double old_mu[12], double new_mu[12]){
    double sum =0;
    for(int i =0; i< (int)(sizeof(old_mu) / sizeof(old_mu[0])) ; i++){
        double power = pow(old_mu[i] - new_mu[i],2);
        sum += power;
    }
    sum = pow(sum,0.5);
    return sum;
}


// max_norm
//input: norms_array
// output: find thw max norm of the array
double compute_max_norm(double norm_array[12]){
    double max_norm =0;
    for (int i =0; i< (int)(sizeof(norm_array) / sizeof(norm_array[0])) ; i++)
    {
        if(norm_array[i]>max_norm){
            max_norm = norm_array[i];
        }
    }
    return max_norm;
}



//compute_norms
//input: mus_array, new_mus_array
// output: create norm_array and return max norm
double[12] compute_norm(double[12][K] mus_array, double[12][K] new_mus_array ){
    double cord_norm [12];
    for (int i =0; i< (int)(sizeof(mus_array) / sizeof(mus_array[0])) ; i++){
        cord_norm[i] = euqlide_norm(mus_array[i], new_nus_array[i]);
    }
    return cord_norm;

}


//write_output_file
//input: mus_array
//output: create output.txt and write mus_array in it



void K_mean(int K, char* filename, int max_iter = 200){


}




int main(void){
    int K = 12;
    double epsilon = 0.0001;
    int max_norm = epsilon;
    int max_iter = 200;

    FILE *fp;
    fp = fopen("input1.txt","r");
    if (fp == NULL){
        printf("falid open file");
        return 1;
    }
    struct vertex all_x_array[12]; // we have to give an initial number of x in file, we can allocate mmemory evrey time we run of space
    initial_all_x_array(all_x_array,*fp);
    double mus_array[K][K];
    initialze_mus_array(mus_array);
    double new_mus_array[K][k];
    
    int counter =0;
    while(max_norm>= epsilon || counter <= max_iter){
        counter++;
        update_mu_for_all_vertex(all_x_array);
        new_mus_array[K][K] = { 0 };
        update_new_mu(new_mus_array);
        double norms_array = compute_norms(mus_array,new_mus_array);
        max_norm = compute_max_norm(norms_array);
        mus_array = new_mus_array;

    }
    write_to_output(mus_array);


///////



}