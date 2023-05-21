# **P-FHDI**

Parallel fractional hot-deck imputation (P-FHDI) is a general-purpose, assumption-free tool for handling item nonresponse in big incomplete data by combining the theory of FHDI and parallel computing. It inherits all the strengths of the _R_ Package FHDI. P-FHDI can tackle big incomplete data with up to millions of instances or 10,000 variables. 

- [Benchmarks](#Benchmarks)
- [Usage](#Usage)
  - [Dependencies](#Dependencies)
  - [Command](#Command)
- [Citation](#Citation)
- [Acknowledgements](#Acknowledgements)

Please see the tutorial to illustrate the use of P-FHDI with benchmarks in [P-FHDI paper](https://ieeexplore.ieee.org/document/9214981).

# Benchmarks
Please see benchmarks in [Parallel FHDI](https://sites.google.com/site/ichoddcse2017/home/type-of-trainings/parallel-fhdi-p-fhdi-1).

| Dataset  | Variable type | # Instances | # Variables | Missing rate |   Source |
| :---: | :---: | :---: | :---: | :---: |  :---: |
| Synthetic 1  | Continous | 1000  | 4 | 25% | Synthetic |
| Synthetic 2  | Continous | 1M  | 4 | 25% | Synthetic |
| Air Quality  | Hybrid | 41757  | 4 | 10% | UCI |
| Nursery  | Categorical | 12960  | 5 | 30% | UCI |
| Synthetic 3  | Continous | 15000  | 12 | 15% | Synthetic |
| Synthetic 4  | Continous | 15000  | 16 | 15% | Synthetic |
| Synthetic 5  | Continous | 15000  | 100 | 15% | Synthetic |
| Synthetic 6  | Continous | 1000  | 100 | 30% | Synthetic |
| Synthetic 7  | Continous | 1000  | 1000 | 30% | Synthetic |
| Synthetic 8  | Continous | 1000  | 10000 | 30% | Synthetic |
| Appliance Energy  | Continous | 19735  | 26 | 15% | UCI |

# Usage

## Dependencies

- Intel MPI
- Access to HPC Facilities


## Command
P-FHDI has been broadly validated on Condo2017, a free high-performance computing (HPC) facility open to iowa state university faculties. Users can run the following commands to deploy P-FHDI on Condo2017. 

Load the Intel compiler:
```linux
module load intel/18.3
```

Compile P-FHDI program:
```linux
mpiicc –o main_MPI main_MPI.cpp
```

Submit a job script to launch MPI code with requested computing resources
```linux
sbatch run.sbatch
```
Note that users should carefully investigate how to deploy P-FHDI on other HPC facilities! 

# Citation
Please kindly cite the following papers if you find this software useful. Thanks!
- Yicheng Yang, Jaekwang Kim, and In Ho Cho, 2020. [Parallel fractional hot-deck imputation and variance estimation for big incomplete data curing](https://ieeexplore.ieee.org/document/9214981), _IEEE Transactions on Knowledge and Data Engineering_ 34(8), 3912-3926 [DOI: 10.1109/TKDE.2020.3029146].
- Jongho Im,	In Ho Cho, and Jaekwang Kim, 2018. [FHDI: an R package for fractional hot-deck imputation for multivariate missing data](https://journal.r-project.org/archive/2018/RJ-2018-020/index.html), _The R Journal_ 10(1), 140-154 [DOI: 10.32614/RJ-2018-020].

```latex
@Article{Yicheng:2020,
  author = {Yicheng Yang and Jaekwang Kim and In Ho Cho},
  title = {Parallel fractional hot deck imputation and variance estimation for big incomplete data curing},
  journal = {IEEE Transactions on Knowledge and Data Engineering},
  year = {2020},
  volume = {34},
  number = {8},
  pages = {3912--3926},
  doi = {10.1109/TKDE.2020.3029146},
}

@Article{RJ-2018-020,
  author = {Jongho Im and In Ho Cho and Jae Kwang Kim},
  title = {FHDI: An R Package for Fractional Hot Deck Imputation},
  year = {2018},
  journal = {The R Journal},
  url = {https://doi.org/10.32614/RJ-2018-020},
  volume = {10},
  number = {1},
  pages = {140--154},
  doi = {10.32614/RJ-2018-020},
}
```

# Acknowledgements

This software was supported by the research funding of Department of Civil, Construction, and Environmental Engineering of Iowa State University. The high-performance computing facility used for this software was partially supported by the HPC@ISU equipment at ISU, some of which has been purchased through funding provided by National Science Foundation under MRI grant number CNS 1229081 and CRI grant number 1205413. This software was also supported by National Science Foundation under grants CBET-1605275 and OAC-1931380.