void correlated_variable_global(const int ncol, const int i_option_collapsing, int* ia_temp,
	double **correlation_yicheng, std::vector<int> &v_mxl, ofstream& TestOut)

	//Description=========================================
	//Select the most i_option_collapsing correlated variables from all observed variables of each mox
	//Algorithm:
	//Select the most correlated variables using majority vote. The last one is selected based on the highest correlation

	//IN    : int i_option_collapsing = choice of big-p algorithm 
	//                               0= no big-p algorithms
	//                              !0= perform big-p algorithms
	//IN    : int ia_temp(ncol) = copy of mox[i]
	//IN    : double correlation_yicheng(ncol, ncol);// correlation matrix

	//OUT   : int v_mxl(i_option_collapsing); // the actual location of most correlated variables of mox[i]
	//=====================================================

{

	std::vector<int> v_lm; //temporary vector for the locaiton of missing values in mox

	whichINVNOT(ia_temp, ncol, 0, v_lm); //get the actual location of missing variables of mox[i] 

	int v_lm_size = v_lm.size(); //number of missing variables in mox[i]

	std::vector<int> v_lo; //temporary vector for the actual locaiton of observed values in mox[i]

	//for (int a1 = 0; a1 < ncol; a1++) {
	//	for (int a2 = 0; a2 < v_lm_size; a2++) {
	//		if ((a1 + 1) != v_lm[a2]) {
	//			continue;
	//			
	//		}
	//		v_lo.push_back(a1 + 1);
	//	}
	//}
	whichINV(ia_temp, ncol, 0, v_lo); //get the location of Non-zero in mox 

	int v_lo_size = v_lo.size(); //number of observed variables in mox[i]

	//for (int a3 = 0; a3 < v_lo_size; a3++) {
	//	TestOut <<"v_lo["<<a3<<"]: "<< v_lo[a3] << endl;
	//}

	if (v_lm_size + v_lo_size != ncol) {
         TestOut << "Error in correlated_variable_gloabl!!!!!" << endl;
		 return;
	}
		
	//========================================================
	// Pickout the correlation matrix of missing variables only
	//========================================================

	double** correlation_temp = New_dMatrix(v_lm_size, (ncol - v_lm_size));

	//TestOut << "v_lm at mox i=  "<< endl;
	//for (int kk1 = 0; kk1 < v_lm_size; kk1++) {
	//	TestOut << v_lm[kk1] << endl;
	//}

	for (int k6 = 0; k6 < v_lm_size; k6++) {
		for (int k7 = 0; k7 < (ncol - v_lm_size); k7++) {
			correlation_temp[k6][k7] = abs(correlation_yicheng[v_lm[k6] - 1][v_lo[k7] - 1]);
		}
	}


	std::vector<double> r_star; // vector of the highest correlation of each obeserved variable of current mox
	std::vector<double> r_temp; // temporary vector to hold coreelations of between an observed variable and all missing variables

	//=================================
	// Select out the vector of the highest correlation of each obeserved variable of current mox
	for (int k8 = 0; k8 < v_lo_size; k8++) {
		r_temp.clear();
		for (int k9 = 0; k9 < v_lm_size; k9++) {
			r_temp.push_back(correlation_temp[k9][k8]);
		}

		r_star.push_back(max_FHDI(r_temp));

	}

	//for (int b1 = 0; b1 < r_star.size(); b1++) {
	//	TestOut <<"r_star["<<b1<<"]: "<< r_star[b1] << endl;
	//}
	//===================================
	// Add variables whose correlation is among the top of the largest i_option_collapsing

	for (int k10 = 0; k10 < i_option_collapsing; k10++) {
		int max_corr = 0;

		for (int k11 = 0; k11 < v_lo_size; k11++) {
			if (r_star[max_corr] < r_star[k11]) {
				max_corr = k11;
			}
		}

		//TestOut<<"max_corr: "<< max_corr <<endl;
		v_mxl.push_back(v_lo[max_corr]); // the actual location of the variable

		r_star[max_corr] = 0;
	}


	sort(v_mxl.begin(), v_mxl.end());
	//TestOut << "correlation_temp2 after max_occur at mox i=  " << i << endl;
	//for (int kk2 = 0; kk2 < v_lm_size; kk2++) {
	//	for (int kk3 = 0; kk3 < (ncol - v_lm_size); kk3++) {
	//		TestOut << setw(20) << correlation_temp2[kk2][kk3];
	//	}
	//	TestOut << endl;
	//}

	//----------------------------------
	//Del_iMatrix(correlation_temp, v_lm_size, (ncol - 1));
	Del_dMatrix(correlation_temp, v_lm_size, (ncol - v_lm_size));

	return;
}