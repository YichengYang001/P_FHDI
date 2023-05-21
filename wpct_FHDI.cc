void wpct_FHDI(std::string s_0[], const int n, const double* w, 
               std::vector<std::string> &jp_name, std::vector<double> &jp_prob)
//Description=====================================
//  calculate weighted probability of the string array 
//  using the given weight array in w[]
//
//  Written by Dr I. Cho
//  All right reserved
//
//  Algorithm: similar to "R" wpct()
//
//IN   : string s_0[n] 	= target array of string. e.g., all the observed rows of condensed strings
//IN   : double w[n]  	= user-defined weight used for proportional weights
//OUT  : std::vector<std::string> jp_name  = names of joint probability table
//OUT  : std::vector<double>      jp_prob  = weighted joint probability of the table 
//================================================
{
	
	//---------------
	//make a table of s_0[n]
	//---------------
	std::vector<std::string> v_table_row1; //names of the table
	std::vector<int> 		 v_table_row2; //counts of the table
	table_cpp(s_0, n, v_table_row1, v_table_row2);
	const int i_size_v_table = (int)v_table_row2.size();	
	
	//---------------
	//find new accumulated weights for each category
	//---------------
	double* d_weight = new double[i_size_v_table];
	Fill_dVector(d_weight, i_size_v_table, 0.0);
	
	std::string s_temp; 
	int i_count=0; 
	for(int i=0; i<i_size_v_table; i++) //loop for table names 
	{
		s_temp = v_table_row1[i]; 
		i_count = 0; //re-initialize 
		
		//-----------
		//search and get the weight of current string
		//-----------
		for(int j=0; j<n; j++)
		{
			if(s_temp.compare(s_0[j]) == 0) //0 means equal string
			{
				d_weight[i] = d_weight[i] + w[j];  //accumulate the weight of this category
				i_count++; 
				if(i_count == v_table_row2[i]){break;} 
			}
		}
	}	
	
	//-----------------
	//sum of d_weight 
	//-----------------
	double d_sum_w = 0.0; 
	for(int i=0; i<i_size_v_table; i++) d_sum_w += d_weight[i]; 
	if(d_sum_w == 0.0) {cout<<"Error! zero sum of weights in wpct from wpct_FHDI.cc"<<endl; return; }
	
	
	//------------------
	//prep return
	//------------------
	for(int i=0; i<i_size_v_table; i++)
	{
		jp_name.push_back(v_table_row1[i]); 
		jp_prob.push_back(d_weight[i]/d_sum_w); 
	}
		
	//------------------
	//Deallocation
	//------------------
	delete[] d_weight; 

} 