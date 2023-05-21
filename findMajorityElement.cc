#include <vector>
#include <map>

vector<double> findMajorityElement(vector< vector<double> > voteCand)
	//================================
	// Description
	//================================
	// 
	// Determine the majority of elements of an array
	//--------------------------------
	//IN	:	double arr[count of elements]	// array of elements
	//IN	:	int size						// size of array
	//OUT	:	double majorityElement			// selected majority element
	//================================
{
	int count = 0;// , i;
	string majorityElement;
	map<vector<double>, int> freqMap; // key (vector<double>), value (int) table // int := frequency

	for (vector< vector<double> >::iterator it = voteCand.begin(); it != voteCand.end(); ++it) {
		map<vector<double>, int>::iterator mapIter;
		mapIter = freqMap.find(*it);
		// If the key does not exist create new key, value pair and insert it to map
		if (mapIter == freqMap.end()) { // *it ---> vector<double>
			freqMap.insert(pair<vector<double>, int>(*it, 1));
		}
		// Otherwise, update value (count)
		else {
			mapIter->second++;
		}
	}

	vector<double> ret;
	int maxCount = 0; // temp for only iteration
	for (map<vector<double>, int>::iterator it = freqMap.begin(); it != freqMap.end(); ++it) {
		if (it->second > maxCount) {
			ret = it->first;
			maxCount = it->second;
		}
	}

	return ret;
}