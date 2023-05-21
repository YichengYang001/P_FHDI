
void Ranking_m(const int nrow, const int ncol, double** x_raw, int** r_raw, double ** correlation_yicheng, int** correlation_ranking, ofstream& TestOut)

//Description=========================================
//Make correlation matrix correlation_yicheng
//Make ranking matrix of each variable according to correlation matrix in descending order

//IN	: double x(nrow, ncol) 	= {y1, y2, ... } total data containing missing values
//IN	: double r(nrow, ncol) 	= {y1, y2, ... } total response inndicators containing missing values
//OUT   : double correlation_yicheng(ncol, ncol);
//OUT   : int correlation_ranking(ncol, ncol-1); // Ranking of correlation of each variable in descending order. 
                                                 // Note it excludes itself from ranking
//=====================================================
	{
		//----------------
		//Prepare fully observed y matrix
		//---------------------
		std::vector<int> ol;
		int d_temp = 0;
		for (int i_row = 0; i_row < nrow; i_row++)
		{
			d_temp = 1.0;
			for (int i_col = 0; i_col < ncol; i_col++)
			{
				if (r_raw[i_row][i_col] == 0) { d_temp = 0.0; break; } //found zero, i.e. missing cell
			}

			if (fabs(d_temp) > 1e-15) //this row has no missing cells
			{
				ol.push_back(i_row);
			} //actual number of the row having no missing cells
		}
		int nrow_ol = ol.size();

		double** ol_matrix = New_dMatrix(nrow_ol, ncol);

		for (int i = 0;i < nrow_ol;i++) {
			for (int j = 0; j < ncol; j++) {
				ol_matrix[i][j] = x_raw[ol[i]][j];
			}
		}
		//TestOut << "ol_matrix[]" << endl;
		//for (int i = 0; i < nrow_ol; i++)
		//{
		//	for (int j = 0; j < ncol; j++) { TestOut << setw(20) << ol_matrix[i][j]; }
		//	TestOut << endl;
		//}

		//-----------------------
		//Compute corrrlation matrix
		//-----------------------

		//double** correlation_yicheng = New_dMatrix(ncol, ncol);
		correlation_FHDI(ol_matrix, nrow_ol, ncol, correlation_yicheng);
		//TestOut << "correlation matrix: " << endl;
		//for (int i = 0; i < ncol; i++) {
		//	for (int j = 0; j < ncol; j++) {
		//		TestOut << setw(20) << correlation_yicheng[i][j];
		//	}
		//	TestOut << endl;
		//}

		//-----------------------
		//Compute ranking matrix 
		//----------------------

		int** correlation_m_temp = New_iMatrix(ncol, (ncol - 1));

		for (int i = 0; i < ncol; i++) {

			double* d_source_temp = new double[ncol];
			int* i_return = new int[ncol]; //order of score actual loc

			for (int j = 0; j < ncol; j++) {
				d_source_temp[j] = abs(correlation_yicheng[i][j]);// Note the ranking of correlation matrix should be based on absolute value !!!
			}
			order_FHDI(d_source_temp, ncol, i_return);

			// Note i_return_temp must exclude itself priorly in case that i_return have several correlations of 1s
			std::vector<int> i_return_temp;
			for (int j1 = 0; j1 < ncol; j1++) {
				if (i_return[j1] != (i + 1)) {
					i_return_temp.push_back(i_return[j1]);
				}
			}

			if (i_return_temp.size() != (ncol - 1)) TestOut << "Error! The ranking of simple correlation is not correct!" << endl;

			for (int k3 = 0; k3 < (ncol - 1); k3++) {
				correlation_m_temp[i][k3] = i_return_temp[k3];
			}

			delete[] d_source_temp;
			delete[] i_return;
		}

		//TestOut << "correlation_m_temp in ascending order" << endl;
		//for (int kk2 = 0; kk2 < ncol; kk2++) {
		//	for (int kk3 = 0; kk3 < (ncol - 1); kk3++) {
		//		TestOut << setw(20) << correlation_m_temp[kk2][kk3];
		//	}
		//	TestOut << endl;
		//}

		// Reverse the ranking matrix in the descending order
		for (int i = 0; i < ncol; i++) {
			for (int j = 0; j < (ncol - 1); j++) {
				correlation_ranking[i][j] = correlation_m_temp[i][ncol - 2 - j];
			}
		}

		//TestOut << "correlation_ranking in descending order" << endl;
		//for (int kk2 = 0; kk2 < ncol; kk2++) {
		//	for (int kk3 = 0; kk3 < (ncol - 1); kk3++) {
		//		TestOut << setw(20) << correlation_ranking[kk2][kk3];
		//	}
		//	TestOut << endl;
		//}

		Del_dMatrix(ol_matrix, nrow_ol, ncol);
		//Del_dMatrix(correlation_yicheng, ncol, ncol);
		Del_iMatrix(correlation_m_temp, ncol, (ncol - 1));
	}

