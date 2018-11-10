#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <iomanip>
#include <algorithm>



// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

long start_time;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

   public:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning
    vector<float> new_CPT;
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		
	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_newCPT(vector<float> temp_CPT)
    {
        new_CPT.clear();
        new_CPT=temp_CPT;
    }
    void initialise_new_CPT()
    {
        for(int i=0;i<new_CPT.size();i++)
        {
            new_CPT[i]=0.0001;
        }

    }
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};




 // The whole network represted as a list of nodes
class network{

	public:
    vector <Graph_Node> Pres_Graph;

   
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        int count=0;
        for(int i=0;i<Pres_Graph.size();i++)
        {
            if(Pres_Graph[i].get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
    // get the node at nth index
    Graph_Node* get_nth_node(int n)
    {
        return &Pres_Graph[n]; 
    }
    //get the iterator of a node with a given name
    Graph_Node* search_node(string val_name)
    {
        int i=0;
        for(i=0;i<Pres_Graph.size();i++)
        {
            if(Pres_Graph[i].get_name().compare(val_name)==0)
                return &Pres_Graph[i];
        }
    
            cout<<"node not found\n";
        return &Pres_Graph[i];
    }
	

};

network read_network(string in_file)
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile(in_file); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    Graph_Node* listIt;
                    Graph_Node* listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    vector<float> temp_CPT;

                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					// curr_CPT.push_back(atof(temp.c_str()));
                        curr_CPT.push_back(0.01);
                        temp_CPT.push_back(0.01);
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    listIt->set_CPT(curr_CPT);
                    listIt->set_newCPT(temp_CPT);


     		}
            else
            {
                
            }  		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}


void print_network(network Alarm, string in_file){
    
    string line;
    stringstream ss;
    string temp;

    ifstream myfile(in_file); 
    ofstream outfile("solved_alarm.bif");

    int idx = 0;
    if (myfile.is_open())
    {
        while (! myfile.eof() )
        {
            string initial("prob");

            getline (myfile,line);
            std::pair<string::iterator,string::iterator> res;
            
             res = mismatch(initial.begin(), initial.end(), line.begin());

            if(res.first != initial.end() && line.size() == 0){
                outfile<<line;
            }
            else if(res.first != initial.end()){
                outfile << line << endl;
            }
            else{
                outfile << line << endl;
                getline(myfile,line);
                ss.str(line);
                ss>>temp;
                outfile << '\t' << temp<<" ";
                Graph_Node* it = Alarm.get_nth_node(idx);
                int state = it->get_nvalues();
                int size_cpt=it->get_CPT().size();
                // while(state_idx < state){
                for(int iter=0;iter<size_cpt;iter++)
                {
                    outfile<<setprecision(5)<<fixed;
                    outfile<<it->get_CPT()[iter]<<" ";   
                }
                    // for(auto &iter : it->get_CPT()){
                        //outfile<<setprecision(4)<<fixed<<iter.second[state_idx]<<" ";
                        
                    // }
                    // state_idx = state_idx + 1;
                // }
                outfile<<';'<<endl;
                idx = idx + 1;
            }
        }
        myfile.close();
        outfile.close();
    }
}

int find_update_index(vector<int> ent_line, int trav , network* Alarm)
{
    Graph_Node* gr=Alarm->get_nth_node(trav);
    int nvalues=gr->get_nvalues();
    vector<string> values=gr->get_values();
    int CPT_size=gr->get_CPT().size();

    int first=trav;

    // for(first=0;first<nvalues;first++)
    // {
    //     if(ent_line[trav].compare(values[first])==0)
    //     {
           
    //         break;
    //     }
    //      // cout<<values[first]<<"yeh hai"<<endl;

    // }
    int change_index_counter=CPT_size*(ent_line[trav])/nvalues;

    // cout<<gr->Parents.size()<<endl;
    vector<string> pars ;
    pars=gr->Parents;
    // used to reach/update the correct dstination/ entry in CPT
    int sizee=CPT_size/nvalues;
    
    // cout<<"here"<<endl;
    for(int i=0;i<pars.size();i++)
    {
        // cout<<"entered for"<<endl;

        Graph_Node* paa=Alarm->search_node(pars[i]);
        // int this_ind=Alarm.get_index(pars[i]);

        int par_ind=Alarm->get_index(pars[i]);
        int par_nvalues=paa->nvalues;

        int j=0;

        sizee=sizee/par_nvalues;
        change_index_counter+=sizee*(ent_line[par_ind]);

        // cout<<"sizee= "<<sizee<<" change_index_counter= "<<change_index_counter<<endl;

    }
    return change_index_counter;

}

// vector<float> prob_finder(vector<int> line, int index, network Alarm)
// {
//     Graph_Node* gr=Alarm.get_nth_node(index);
//     int nvalues=gr->nvalues;
//     // vector<string> values=gr->values;
//     // string initial=line[index];
//     // int initial_index=0;
//     float arr_entries[nvalues];
//     float sum=0;
//     vector<float> answers;

//     for(int j=0;j<nvalues;j++)
//     {
//         // if(values[j].compare(initial)==0)
//         // {
//         //     initial_index=j;
//         // }
//         line[index]=j;
//         int relevant_index=find_update_index(line,index,Alarm);
//         arr_entries[j]=(gr->CPT)[relevant_index];
//         sum+=arr_entries[j];
//     }

//     for(int j=0;j<nvalues;j++)
//     {
//         arr_entries[j]/=sum;
//         answers.push_back(arr_entries[j]);
//     }
//     return answers;
// }

// vector<float> child_prob_finder(vector<int> line, int q_index, int ch_index, network Alarm)
// {
//     Graph_Node* gr=Alarm.get_nth_node(ch_index);
//     Graph_Node* fath=Alarm.get_nth_node(q_index);

//     int f_nvalues=fath->nvalues;
//     // vector<string> f_values=fath->values;
//     int nvalues=gr->nvalues;
//     // vector<string> values=gr->values;

//     int initial=line[ch_index];
//     int initial_index=0;

//     float arr_entries[f_nvalues];
    

//     vector<float> answers;

//     for(int j=0;j<f_nvalues;j++)
//     {
//         float sum=0;
//         line[q_index]=j;
//         float ch_arr[nvalues];
//         for(int k=0;k<nvalues;k++)
//         {

//             line[ch_index]=k;

//             int relevant_index=find_update_index(line,ch_index,Alarm);
//             ch_arr[k]=(gr->CPT)[relevant_index];
//             sum+=ch_arr[k];
            
//         }
//         for(int k=0;k<nvalues;k++)
//         {
//             ch_arr[k]/=sum;
//         }

//         arr_entries[j]=ch_arr[initial];
//     }

//     for(int j=0;j<nvalues;j++)
//     {
//         answers.push_back(arr_entries[j]);
//     }

//     return answers;

// }


// float RandomFloat(float a, float b) 
// {
//     float random = ((float) rand()) / (float) RAND_MAX;
//     float diff = b - a;
//     float r = random * diff;
//     return a + r;
// }

void round_off(network* Alarm)
{
    for(int i=0;i<Alarm->Pres_Graph.size();i++)
    {
        Graph_Node* gr=Alarm->get_nth_node(i);

        int nvalues=gr->nvalues;
        int CPT_size=gr->CPT.size();
        for(int j=0;j<CPT_size;j++)
        {
            if(gr->CPT[j]<0.001)
                gr->CPT[j]=0.001;
        }

    }   
}

network* updater(vector<vector<int> > all_lines, vector<int> q_marks, network* Alarm)
{
    for(int i=0;i<all_lines.size();i++)
    {
        vector<int> line=all_lines[i];
        int q_index=q_marks[i];
        for(int j=0;j<all_lines[i].size();j++)
        {
            if(j==q_index)
            { 
                // int early_index=find_update_index(line, q_index,*Alarm);

                Graph_Node* gr=Alarm->get_nth_node(q_index);
                int nvalues=gr->nvalues;
                vector<string> values=gr->values;
                int CPT_size=gr->CPT.size();
                float arr[nvalues];
                int indices[nvalues];
                float temp_sum=0;

                for(int nn=0;nn<nvalues;nn++)
                {
                    line[q_index]=nn;
                    int index=find_update_index(line,q_index,Alarm);
                    float prob=gr->CPT[index];

                    vector<int> children=gr->Children;

                    for(int j=0;j<children.size();j++)
                    {
                        int temp=find_update_index(line,children[j],Alarm);
                        Graph_Node* ch1=Alarm->get_nth_node(children[j]);

                        float mul=ch1->CPT[temp];
                        prob*=mul;
                    }
                    temp_sum+=prob;
                    arr[nn]=prob;
                    indices[nn]=index;

                }

                for(int nn=0;nn<nvalues;nn++)
                {
                    arr[nn]/=temp_sum;
                    gr->new_CPT[indices[nn]]+=arr[nn];
                }
            }

            else 
            {
                int flag=0;
                Graph_Node* gr=Alarm->get_nth_node(j);
                vector<string> plist=gr->Parents;
                for(int kk=0;kk<plist.size();kk++)
                {
                    if(Alarm->get_index(plist[kk])==q_index)
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag==1)
                {
                    Graph_Node* qq=Alarm->get_nth_node(q_index);
                    int q_nvalues=qq->nvalues;
                   
                    int nvalues=gr->nvalues;
                    vector<string> values=gr->values;
                    int CPT_size=gr->CPT.size();

                    float arr[q_nvalues];
                    int indices[q_nvalues];
                    float temp_sum=0;

                    for(int nn=0;nn<q_nvalues;nn++)
                    {
                        line[q_index]=nn;
                        int index=find_update_index(line,j,Alarm);
                        float prob=gr->CPT[index];

                        vector<int> children=gr->Children;

                        for(int j=0;j<children.size();j++)
                        {
                            int temp=find_update_index(line,children[j],Alarm);
                            Graph_Node* ch1=Alarm->get_nth_node(children[j]);

                            float mul=ch1->CPT[temp];
                            prob*=mul;
                        }
                        temp_sum+=prob;
                        arr[nn]=prob;
                        indices[nn]=index;

                    }

                    for(int nn=0;nn<q_nvalues;nn++)
                    {
                        arr[nn]/=temp_sum;
                        gr->new_CPT[indices[nn]]+=arr[nn];
                    }

                }
                else
                {
                    int index=find_update_index(line,j,Alarm);
                    gr->new_CPT[index]+=1;   
                }

            }
            // vector<float> P_x_par=prob_finder(line, q_index, *Alarm);

        }

    }

    for(int i=0;i<Alarm->Pres_Graph.size();i++)
    {
        Graph_Node* gr1=Alarm->get_nth_node(i);
        gr1->CPT=gr1->new_CPT;
        gr1->initialise_new_CPT();

    }
    return Alarm;


    // return all_lines;
}

void count_to_probs(network* Alarm)
{
    for(int i=0;i<Alarm->Pres_Graph.size();i++)
    {
        Graph_Node* gr=Alarm->get_nth_node(i);
        int nvalues=gr->nvalues;
        int CPT_size=gr->CPT.size();

        int jump=CPT_size/nvalues;

        for(int j=0;j<jump;j++)
        {
            // int arr[nvalues];
            float sum=0;
            int ind=j;
            for(int k=0;k<nvalues;k++)
            {
                sum+=gr->CPT[ind];
                ind+=jump;
            }
            ind=j;
            for(int k=0;k<nvalues;k++)
            {
                // cout << sum << endl ;
                // if(sum==0) cout << "Gaya yeh to" << endl ;
                gr->CPT[ind]/=sum;
                // if(gr->CPT[ind]==NaN)
                //     cout<<"nannananan"<<endl;
                ind+=jump;
            }
            
        }
    }   
}


void makeCPTs(network* Alarm, string record_file)
{
    string line;
    int find=0;
    ifstream myfile(record_file); 
    string temp;
    string name;
    
    if (myfile.is_open())
    {
        vector<int> q_marks;
        vector< vector<string> > all_lines;
        vector<vector<int> > converted;

        while (! myfile.eof() )
        {

            stringstream ss;
            getline (myfile,line);

            ss.str(line);
            int varind=0;
            vector<int> conv_line;
            int temp_index=0;

            while(!ss.eof())
            {
                
                ss>>temp;
                Graph_Node* conv=Alarm->get_nth_node(temp_index);
                vector<string> vec=conv->values;
                int numvalues=conv->nvalues;
                for(int i=0;i<numvalues;i++)
                {
                    if(temp.compare("\"?\"")==0)
                    {
                        conv_line.push_back(0);
                        q_marks.push_back(temp_index);
                        break;
                    }
                    else if(temp.compare(vec[i])==0)
                    {
                        conv_line.push_back(i);
                        break;
                    }
                }
                
                temp_index+=1;
            }
            converted.push_back(conv_line);

            for(int trav=0;trav<conv_line.size();trav++)
            {

                Graph_Node* gr=Alarm->get_nth_node(trav);
                
                vector<float> temp_CPT;
                int CPT_size=gr->get_CPT().size();

                for(int ko=0;ko<CPT_size;ko++)
                {
                    temp_CPT.push_back(0.0001);
                }


                int change_index_counter=find_update_index(conv_line,trav,Alarm);
                // change_index_counter=0;
                temp_CPT[change_index_counter]+=1;
                // gr->set_CPT(temp_CPT);
                // cout<<"heyy";
                for(int ko=0;ko<temp_CPT.size();ko++)
                {
                    gr->CPT[ko]+=temp_CPT[ko];

                }
                // cout<<change_index_counter<<" chnge "<<trav<<" " ;

            }
            // cout<<endl;
   

        }


        count_to_probs(Alarm);

        for(;;)
        {
            time_t now= time(0);
            if(now-start_time>400)
                break;
            Alarm=updater(converted,q_marks, Alarm);
            count_to_probs(Alarm);
  
            
        }
   

    }
    
}

int main(int argc, char* argv[])
{
	network Alarm;
    time_t now=time(0);
    start_time=now;
    // cout<<start_time<<endl;
	Alarm=read_network(argv[1]);

    makeCPTs(&Alarm, argv[2]);
    // count_to_probs(&Alarm);
    round_off(&Alarm);


    print_network(Alarm, argv[1]);


    // for(int i=0;i<Alarm.Pres_Graph.size();i++)
    // {
    //     Graph_Node* gr1=Alarm.get_nth_node(i);
    //     for(int j=0;j<gr1->CPT.size();j++)
    //     {
    //         cout<<gr1->CPT[j]<<" ";
    //     }
    //     cout<<endl;
    // }

	cout<<"Perfect! Hurrah! \n";
	
}




