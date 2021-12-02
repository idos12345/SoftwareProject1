#include <stdio.h>
#include<math.h>
#include <string.h>
#include <stdlib.h>
// desktop
typedef struct node
{
    double* xi;
    struct node* next;
    struct node* prev;
} node;

typedef struct mu{
    double* mui;
    node* xi_list;
} mu;

typedef struct change{
    node* xi_node;
    int old_mu_index;
    int new_mu_index;
} change;

/* function's decleration*/
double** initial_all_x_array(FILE* fp, int number_of_cord, int number_of_lines);
mu* initialze_mus_array(double** X, int K, int number_of_cord);
int argmin(double* xi, mu* mus, int K, int number_of_cords);
void add_x_to_mu(node* xi_node, mu* mui);
void delete_x_from_mu(node* xi_node, mu* mui);
void swap(node* xi_node, int old_mu, int new_mu, mu* mus);
double euqlide_norm(double* old_mu, double* new_mu, int number_of_cords);
double update_mus(mu* mus, int K, int number_of_cords);
int write_to_outputfile(mu* mus, int K, char* outputfile, int number_of_cords );
int compute_number_of_x(FILE *fp );
int K_mean(int K, int max_iter, char* filename, char* outputfile);
void initial_xi_liked_list( int number_of_lines, int K, int number_of_cords, mu* mus, double** X);
change* update_changes_array(mu* mus, change* change_array, int number_of_cords, int K );
void implementing_changes(mu* mus, int number_of_lines, change* change_array);


/* create an array of pointers to xi calld X
 output: read file and update  all_x_array -  an array of vertex , initial all mus to null*/
double** initial_all_x_array(FILE* fp, int number_of_cord, int number_of_lines){
     double** X = (double**)calloc(number_of_lines , sizeof(double*));
     rewind(fp);
     int line_number;
     double* xi;
     char* line;
     char* curr_place_in_line;
        for(line_number =0 ; line_number < number_of_lines; line_number++)
        {
            line = (char*)calloc(1024,sizeof(char));
            fscanf(fp, "%s",line);
            xi = (double*) calloc(number_of_cord,sizeof(double));
            for ( int i =0; i<number_of_cord -1; i++){
                curr_place_in_line = strchr((char*)line,','); /* point to the first , in line */
                *curr_place_in_line = '\0'; 
                xi[i] = atof(line); /* insert word to xi */
                line = ++curr_place_in_line; /* update line to tne next char after , */
            }
            xi[number_of_cord-1] = atof(line);
            X[line_number]= xi;  
            
        }
        return X;
}

/* create mu_array from first K xi from X
 output: return mu_array - the firt K xi from X */
mu* initialze_mus_array(double** X, int K, int number_of_cord){
    mu* mus = (mu*)calloc(K,sizeof(mu));
    int i, word;
    for(i =0; i< K; i++){
        mus[i].mui= (double*)calloc(number_of_cord,sizeof(double));
        mus[i].xi_list = NULL;
        for(word =0; word < number_of_cord; word++){
            mus[i].mui[word] = X[i][word];
        }
    }
    return mus;
}

/* outpot: the index of its clostest mu of xi*/
int argmin(double* xi, mu* mus, int K, int number_of_cords){
   double min_sum, sum;
   int min_index=0, mu_index, cord;

    for(mu_index =0; mu_index < K; mu_index++ ){
       sum =0;
       for(cord = 0; cord< number_of_cords; cord++){
           sum += pow((xi[cord] - mus[mu_index].mui[cord]),2);
       }
       if (mu_index ==0) min_sum = sum;
        else if(sum<min_sum) {
            min_sum = sum;
            min_index = mu_index;
        } 
    }
    return min_index;
}

/* output: add node of xi to xi_list of mui*/
void add_x_to_mu(node* xi_node, mu* mui){
    xi_node->prev = NULL;
    if(mui->xi_list == NULL){ /* xi_list empty*/
        mui->xi_list = xi_node; 
        xi_node->next = NULL;
    }else{
    mui->xi_list->prev = xi_node;
    xi_node->next = mui->xi_list;
    mui->xi_list = xi_node;
    }
}

/* output: remove node ox xi frox xi_list of mui*/
void delete_x_from_mu(node* xi_node, mu* mui){
    if(xi_node->prev == NULL){ /* first node in list*/
        if(xi_node->next == NULL){ /* xi_node is the only node in list*/
            mui->xi_list = NULL;
        }else{
            xi_node->next->prev = NULL;
            mui->xi_list= xi_node->next;
        }
    } 
    else if (xi_node->next ==NULL) /* last node in list*/
    {
        xi_node->prev->next=  NULL;
    }
    else{ 
        xi_node->prev->next = xi_node->next;
        xi_node->next->prev = xi_node->prev;
    }
}

/* output: remote xi_node from old_mu list and add it to the start of new_mu list*/
void swap(node* xi_node, int old_mu, int new_mu, mu* mus){
    delete_x_from_mu(xi_node, &mus[old_mu]);
    add_x_to_mu(xi_node,&mus[new_mu]);
}

/*euqlide_norm
output: for every cord (sum of (old_mu[cord]**2 -new_mu[cord])**2 )**0.5*/
double euqlide_norm(double* old_mu, double* new_mu, int number_of_cords){
    double sum =0, power;
    int i;
    for(i =0; i< number_of_cords ; i++){
        power = pow(old_mu[i] - new_mu[i],2);
        sum += power;
    }
    sum = pow(sum,0.5);
    return sum;
}

/* output: compute and update new_mus_array and returns delta max */
double update_mus(mu* mus, int K, int number_of_cords){
    double deltamax =0, delta;
    int mu_index, xi_list_len, cord;
    double* new_mu;
    double* old_mu;
    node* curr_xi;
    for(mu_index =0; mu_index< K; mu_index++){
        xi_list_len =0;
        new_mu = (double*)calloc(number_of_cords, sizeof(double));
        curr_xi = mus[mu_index].xi_list;
        while (curr_xi != NULL)
        {
            for(cord =0; cord<number_of_cords; cord++){
                new_mu[cord] += curr_xi->xi[cord];
            }
            xi_list_len++;
            curr_xi = curr_xi->next;
        }
        
        for(cord =0; cord< number_of_cords; cord++){
            new_mu[cord] = new_mu[cord]/xi_list_len;
        }
        old_mu =  mus[mu_index].mui;
        delta = euqlide_norm(old_mu, new_mu, number_of_cords);
        if (delta > deltamax) deltamax= delta;
        mus[mu_index].mui = new_mu;
    }
    return deltamax;
}

/* output: create output.txt and write mus_array in it*/
int write_to_outputfile(mu* mus, int K, char* outputfile, int number_of_cords ){
    int mu, cord;
    FILE *fp;
    fp = fopen(outputfile,"w");
    if(fp == NULL){
        printf("falid open file");
        return 1;
    }
    for(mu =0; mu <K; mu++){
        for(cord =0; cord< number_of_cords; cord++){
             fprintf(fp,"%.4f",mus[mu].mui[cord]);
            if (cord == number_of_cords -1 && mu != K-1 )
            {
                fprintf(fp,"%s","\n");
            }else if (cord != number_of_cords-1)
            {
                fprintf(fp,"%s",",");
            }     
        }
    }
    fprintf(fp,"%s","\n");
    fclose(fp);
    return 0; 
}

/*output: returns number of lines*/
int compute_number_of_x(FILE *fp ){
    rewind(fp); /* goes to the beginning of th file*/
    char ch;
    int xi_counter =0;
     do{
        ch = fgetc(fp);
        if(ch =='\n') xi_counter ++;
    } while (ch != EOF);
    return xi_counter ;
}

/* output: return the number of the coordinates in every x*/
int compute_number_of_cord(FILE *fp){
    rewind(fp); /* goes to the beginning of th file*/
    char ch;
    int cords_counter =1;
    while (ch!='\n')
    {
        ch = fgetc(fp);
        if(ch == ',') cords_counter++;
    }
  return cords_counter;
}

/*create node for each xi and insert it to it's closest mu linked list */
void initial_xi_liked_list( int number_of_lines, int K, int number_of_cords, mu* mus, double** X){
    int xi, new_mu;
    node* new_xi_node;
    for(xi = 0; xi< number_of_lines; xi++){
        new_mu = argmin(X[xi],mus,K,number_of_cords);
        new_xi_node = (node*)malloc(sizeof(node));
        new_xi_node->xi = X[xi];
        add_x_to_mu(new_xi_node, &mus[new_mu]);
    }
}

/* for evry xi save its old mu & new mu in changes array */ 
change* update_changes_array(mu* mus, change* change_array, int number_of_cords, int K ){
    int new_mu, mu_index, xi_counter =0;
    node* curr_xi;
    for(mu_index =0; mu_index< K; mu_index++){
            curr_xi = mus[mu_index].xi_list;
            while (curr_xi != NULL)
            {
                new_mu = argmin(curr_xi->xi,mus,K,number_of_cords);
                change_array[xi_counter].new_mu_index = new_mu;
                change_array[xi_counter].old_mu_index=mu_index;
                change_array[xi_counter].xi_node = curr_xi;
                xi_counter++;
                curr_xi = curr_xi->next;
            }
    }
    return change_array;
}

/* update mu's linked list according changes array */ 
void implementing_changes(mu* mus, int number_of_lines, change* change_array){
    int change_num,new_mu,old_mu;
    for(change_num = 0; change_num< number_of_lines; change_num++){
            new_mu = change_array[change_num].new_mu_index;
            old_mu = change_array[change_num].old_mu_index;
            if(new_mu != old_mu ){
                swap(change_array[change_num].xi_node, old_mu,new_mu, mus);
            }
        }
}

/* how to set defult max_iter = 200??*/
int K_mean(int K, int max_iter, char* filename, char* outputfile){ 
    /* variable's decleration */ 
    double epsilon = 0.001, maxdelta = epsilon;
    int xi, new_mu, mu_index,change_num, old_mu, iter =0;
    node* curr_xi;
    change* change_array;
    node* new_xi_node;
    FILE *fp;

    /* open input file for reading */
    fp = fopen(filename,"r");
    if (fp == NULL){
        printf("falid open file");
        return 1;
    }

    int number_of_cords = compute_number_of_cord(fp);
    int number_of_lines = compute_number_of_x(fp);
    double** X =initial_all_x_array(fp,number_of_cords,number_of_lines);
    mu* mus = initialze_mus_array(X,K,number_of_cords);
    initial_xi_liked_list(number_of_lines, K, number_of_cords, mus,X);
    
    while (iter < max_iter && maxdelta >= epsilon)
    {
        change_array =(change*) calloc(number_of_lines,sizeof(change)); /* Initialized array of changes  */
        change_array = update_changes_array(mus,change_array,number_of_cords,K); /* update changes_array according new mus */
        implementing_changes(mus, number_of_lines,change_array); /* link evrey xi to its new mu according to changes_array */ 
        maxdelta = update_mus(mus,K,number_of_cords); /* compute new max delts */ 
        iter++;
    }
    fclose(fp);
    write_to_outputfile(mus,K,outputfile,number_of_cords);
    return 0;
}

int main(void){ 
    int K = 15;
    char* fileneam = "input_3.txt";
    char* outputname = "output_3.txt";
    K_mean(K,300,fileneam, outputname);
    return 1;
}