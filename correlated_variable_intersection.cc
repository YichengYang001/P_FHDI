void max_occur(std::vector<int> v_table_name, std::vector<int> v_table_counts, int ncol, int size_i, std::vector<int> v_lm, int v_lm_size, int b1,
	const int i_option_collapsing, std::vector<int> &v_mxl, double** correlation_yicheng, int** correlation_temp2, ofstream& TestOut)

//Description=========================================
//Select all variables whose votes reaching i_option_collapsing
//Algorithm:
//If number of variables whose votes reaching i_option_collapsing is smaller than number of required correlated variables left
//select all variables whose votes reaching i_option_collapsing
//If number of variables whose votes reaching i_option_collapsing is larger than number of required correlated variables left
//select variables whose votes reaching i_option_collapsing with the highest correlation

//IN    : int i_option_collapsing = choice of big-p algorithm 
//                            0= no big-p algorithms
//                           !0= perform big-p algorithms
//IN    : int v_table_name = unique ranking name
//IN    : int v_table_counts = corresponding counts of unique ranking name
//IN    : int size_i = number of unique ranking names
//IN    : int v_lm = actual location of missing variables of mox[i]
//IN    : int v_lm_size = number of missing variables of mox[i]
//IN    : int b1 = cursor of the "tank"
//IN    : double correlation_yicheng(ncol, ncol);// correlation matrix
//IN    : int correlation_temp2( v_lm_size, (ncol - v_lm_size) ); // correlation ranking matrix of missing variables neglecting itself
//OUT   : int v_mxl(i_option_collapsing); // the actual location of most correlated variables of mox[i]
//=====================================================

{
	int i_size_v_mxl = v_mxl.size();
	int i_size_left = i_option_collapsing - i_size_v_mxl; // number of required correlated variables left

	std::vector<int> location;// location of all variables reaching v_lm_size occurance

	//---------------
	for (int i = 0; i < size_i; i++) {
		if ((v_table_counts[i] == v_lm_size) && (v_table_name[i] != 0)) {
			location.push_back(i);
			//TestOut<<"locations: "<< i <<endl;
		}
	}

	int location_size = location.size();

	if (location_size == 0) TestOut<<"No variables qulified at current tank"<<endl;
	//---------------
	//Case 1: if number of qualified variables (reaching v_lm_size occurance) is less than number of required correlated variables left
	if ((location_size < (i_size_left+1))&&(location_size>0)) { //location_size <= i_size_left

		for (unsigned i = 0; i < location.size();i++) {
			v_mxl.push_back(v_table_name[location[i]]); // add all of them

			for (int k3 = 0; k3 < v_lm_size; k3++) {// set the added one as 0s in original ranking matrix
				for (int k4 = 0; k4 < (ncol - v_lm_size); k4++) {
					if (correlation_temp2[k3][k4] == v_table_name[location[i]]) {
						correlation_temp2[k3][k4] = 0;
					}

				}
			}
			//if (v_mxl.size() == i_option_collapsing) break;
		}
	}//end of the first case

	//--------------
	//Case 2: if number of qualified variables (reaching v_lm_size occurance) is larger than number of required correlated variables left
	if (location_size > i_size_left) {
		
		//TestOut<<"Yicheng here"<<endl;

		//double* d_max = new double[location_size]; // max correlation of missing variables between each qualified variable
		std::vector<double> d_max;

		for (int i = 0; i < location_size; i++) {
			//double* d_temp = new double[v_lm_size];
			std::vector<double> d_temp;

			for (int j = 0; j < v_lm_size; j++) {

				//TestOut << "row: " << v_table_name[location[i]] - 1 << ", col:" << v_lm[j] << endl;

				//int ou = v_table_name[location[i]] - 1;
				d_temp.push_back( abs(correlation_yicheng[v_table_name[location[i]]-1][v_lm[j]-1]) ); // Note 1. location and v_lm have the actual locations 2. Must compare absolute value of correlation
				//TestOut<<"d_temp["<<i<<"]["<<j<<"]: "<< d_temp[j] <<endl;
			}
			d_max.push_back( max_FHDI(d_temp) );
			//TestOut<<"d_max["<<i<<"]: "<< d_max[i]<<endl;
			//delete[] d_temp;
		}

		//Pick i_size_left qualified variables among all of qualified variables
		for (int k = 0; k < i_size_left; k++) {

			int max_l = 0;
			for (int i = 0; i < d_max.size(); i++) {

				if (d_max[max_l] < d_max[i]) {
					max_l = i;
				}
			}

			d_max[max_l] = 0;
			//TestOut<<"We choose "<< v_table_name[location[max_l]] <<endl;
			v_mxl.push_back(v_table_name[location[max_l]]); // add the one with max correlation 

            // set the added one as 0s in original ranking matrix
			for (int k3 = 0; k3 < v_lm_size; k3++) {
				for (int k4 = 0; k4 < (ncol - v_lm_size); k4++) {
					if (correlation_temp2[k3][k4] == v_table_name[location[max_l]]) {
						correlation_temp2[k3][k4] = 0;
					}

				}
			}


		}

		//delete[] d_max;
	}//end of the second case

	return;

}



	void correlated_variable_intersection(const int ncol, const int i_option_collapsing, int i, int* ia_temp,
	double **correlation_yicheng, int** correlation_ranking, std::vector<int> &v_mxl, ofstream& TestOut)

//Description=========================================
//Select the most i_option_collapsing correlated variables from all observed variables of each mox
//Algorithm:
//Select the most correlated variables using majority vote. The last one is selected based on the highest correlation

//IN    : int i_option_collapsing = choice of big-p algorithm 
//                            0= no big-p algorithms
//                           !0= perform big-p algorithms
//IN    : int ia_temp(ncol) = copy of mox[i]
//IN    : double correlation_yicheng(ncol, ncol);// correlation matrix
//IN    : int correlation_ranking(ncol, ncol-1); // Ranking of correlation of each variable in descending order
                                                 // Note it excludes itself from ranking
//OUT   : int v_mxl(i_option_collapsing); // the actual location of most correlated variables of mox[i]
//=====================================================

	{
		std::vector<int> v_lm; //temporary vector for the locaiton of missing values in mox

		whichINVNOT(ia_temp, ncol, 0, v_lm); //get the actual location of missing variables of mox[i] 

		int v_lm_size = v_lm.size(); //number of missing variables in mox[i]

		//========================================================
		// Pickout the correlation matrix of missing variables only
		//========================================================

		int** correlation_temp = New_iMatrix(v_lm_size, (ncol - 1)); // correlation ranking matrix of missing variables neglecting itself

		//---------------------
		// correlation ranking matrix of missing variables neglecting all missing variable cells
		// this ranking matrix is even
		int** correlation_temp2 = New_iMatrix(v_lm_size, (ncol - v_lm_size)); 
		
		//TestOut << "v_lm at mox i=  " << i << endl;
		//for (int kk1 = 0; kk1 < v_lm_size; kk1++) {
		//	TestOut << v_lm[kk1] << endl;
		//}

		//========================================================
		// Remove all missing variable cells from the correlation matrix of missing variables
		//========================================================

		// Pickout the correlation ranking matrix of missing variables only
		for (int k1 = 0; k1 < v_lm_size; k1++) {
			for (int k2 = 0; k2 < (ncol - 1); k2++) {
				correlation_temp[k1][k2] = correlation_ranking[v_lm[k1] - 1][k2];
			}
		}

		for (int k3 = 0; k3 < v_lm_size; k3++) {
			for (int k4 = 0; k4 < (ncol - 1); k4++) {
				for (int k5 = 0; k5 < v_lm_size;k5++) {
					if (correlation_temp[k3][k4] == v_lm[k5]) {
						correlation_temp[k3][k4] = 0;
					}
				}

			}
		}

		//TestOut << "correlation ranking matrix removing all missing variables (containing 0s) at mox i=  " << i << endl;
		//for (int kk2 = 0; kk2 < v_lm_size; kk2++) {
		//	for (int kk3 = 0; kk3 < (ncol - 1); kk3++) {
		//		TestOut << setw(20) << correlation_temp[kk2][kk3];
		//	}
		//	TestOut << endl;
		//}

		//Remove all missing variables from ranking matrix
		for (int k6 = 0; k6 < v_lm_size; k6++) {
			for (int k7 = 0; k7 < (ncol - v_lm_size); k7++) {
				for (int k8 = 0; k8 < (ncol - 1); k8++) {
					if (correlation_temp[k6][k8] != 0) {
						correlation_temp2[k6][k7] = correlation_temp[k6][k8];
						correlation_temp[k6][k8] = 0;
						break;
					}
				}
			}
		}

		Del_iMatrix(correlation_temp, v_lm_size, (ncol - 1));
		//TestOut << "correlation ranking matrix removing all missing variables at mox i=" << i << endl;
		//for (int kk2 = 0; kk2 < v_lm_size; kk2++) {
		//	for (int kk3 = 0; kk3 < (ncol - v_lm_size); kk3++) {
		//		TestOut << setw(20) << correlation_temp2[kk2][kk3];
		//	}
		//	TestOut << endl;
		//}

		//=====================================================
		//Get the most i_option_collapsing correlated variables
		//======================================================

		std::vector<int> v_table_name;
		std::vector<int> v_table_counts;

		//std::vector<int> v_table_name1;
		//std::vector<int> v_table_counts1;

		for (int b1 = (i_option_collapsing-1); b1 < (ncol - v_lm_size); b1++) {
			v_table_name.clear();
			v_table_counts.clear();
			int counter1 = 0;
			int* cor_temp = new int[(b1 + 1)*v_lm_size]; // the vector to store the 'tank' temporaryly 

			for (int a1 = 0; a1 < (b1 + 1); a1++) {

				for (int a2 = 0; a2 < v_lm_size; a2++) {
					cor_temp[counter1] = correlation_temp2[a2][a1];
					//TestOut << "cor_temp[" << counter1 << "]: " << correlation_temp2[a2][a1] << endl;
					counter1++;
				}
			}

			//----------------
			//This table function is only used for correlated variables
			table_cpp_yicheng(cor_temp, (b1 + 1)*v_lm_size, v_table_name, v_table_counts,TestOut);

			delete[] cor_temp;

			int size_i = v_table_counts.size();

			//for (int c1 = 0; c1 < size_i; c1++) {
			//	TestOut<< "v_table_name: " << v_table_name[c1] <<", v_table_counts: "<< v_table_counts[c1] <<endl;
			//}

			max_occur(v_table_name, v_table_counts, ncol, size_i, v_lm, v_lm_size, b1, i_option_collapsing, v_mxl, correlation_yicheng, correlation_temp2, TestOut);

			if (v_mxl.size() == i_option_collapsing) break;

		}


		sort(v_mxl.begin(),v_mxl.end());
		//TestOut << "correlation_temp2 after max_occur at mox i=  " << i << endl;
		//for (int kk2 = 0; kk2 < v_lm_size; kk2++) {
		//	for (int kk3 = 0; kk3 < (ncol - v_lm_size); kk3++) {
		//		TestOut << setw(20) << correlation_temp2[kk2][kk3];
		//	}
		//	TestOut << endl;
		//}

		//----------------------------------
		//Del_iMatrix(correlation_temp, v_lm_size, (ncol - 1));
		Del_iMatrix(correlation_temp2, v_lm_size, (ncol - v_lm_size));

		return;
	}



