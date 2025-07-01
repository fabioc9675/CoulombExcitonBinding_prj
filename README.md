# Coulomb Exciton Binding calculation ()CoulombExcitonBinding_prj)

This repository contains all the code and the source files to calculate the exciton binding in a quantum well using both elliptical integration and FEM in Comsol

---

## Installation

To compile te C source file use the following command in terminal (MACOS)

`gcc -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include exciton_openmp.c -o exciton_openmp -L/usr/local/opt/libomp/lib -lomp`

### Prerequisites

- **Python 3.10+** (tested on Python 3.11.5)

### 1. Clone the Repository

```bash
git clone https://github.com/fabioc9675/CoulombExcitonBinding_prj.git
```

### Install Python Dependencies

```bash
pip install -r requirements.txt
```

### Usage

This code is used to compare the performance of calculation of Coulomb binding energy for exciton with different conditions

### Repository Structure

```

CoulombExcitonBinding_prj
├── Comsol
│   └── doble_barrera_electric_Field.mph           # Comsol FIle for simulation
├── Coulomb
│   ├── DataPrep
│   │   ├── dat_comsol.txt
│   │   ├── drop_naval.py                          # Clean the NAN data from exported grid from Comsol
│   │   ├── ejecucion.py                           # File to execute the coulomb binding calculation
│   │   ├── exciton_openmp
│   │   ├── exciton_openmp.c                       # C/OpenMP file to calculate the Coulomb binding
│   │   ├── resultados_1.dat   
│   │   ├── xe_1.txt
│   │   ├── xe_10.txt
│   │   ├── xe_5.txt
│   │   ├── xh_1.txt
│   │   ├── xh_10.txt
│   │   └── xh_5.txt
│   ├── DataSets
│   │   ├── acceptor                               # Images from exciton with acceptor impurity
│   │   │   ├── E001.png
│   │   │   ├── E002.png
│   │   │   ├── E003.png
│   │   │   ├── E004.png
│   │   │   ├── E301.png
│   │   │   ├── E302.png
│   │   │   ├── E303.png
│   │   │   ├── E304.png
│   │   │   ├── E401.png
│   │   │   ├── E402.png
│   │   │   ├── E403.png
│   │   │   ├── E404.png
│   │   │   ├── E501.png
│   │   │   ├── E502.png
│   │   │   ├── E503.png
│   │   │   └── E504.png
│   │   ├── donnor                                 # Images from exciton with donnor impurity
│   │   │   ├── E009.png
│   │   │   ├── E010.png
│   │   │   ├── E011.png
│   │   │   ├── E012.png
│   │   │   ├── E309.png
│   │   │   ├── E310.png
│   │   │   ├── E311.png
│   │   │   ├── E312.png
│   │   │   ├── E409.png
│   │   │   ├── E410.png
│   │   │   ├── E411.png
│   │   │   ├── E412.png
│   │   │   ├── E509.png
│   │   │   ├── E510.png
│   │   │   ├── E511.png
│   │   │   └── E512.png
│   │   ├── e_m-1_B10_E5.png
│   │   ├── e_m-1B0E0.png
│   │   ├── e_m0_B10_E5.png
│   │   ├── e_m0B0E0.png
│   │   ├── e_m1_B10_E5.png
│   │   ├── e_m1B0E0.png
│   │   ├── h_m-1_B10_E5.png
│   │   ├── h_m-1B0E0.png
│   │   ├── h_m0_B10_E5.png
│   │   ├── h_m0B0E0.png
│   │   ├── h_m1_B10_E5.png
│   │   ├── h_m1B0E0.png
│   │   ├── neutral                               # Images from exciton without impurity
│   │   │   ├── E005.png
│   │   │   ├── E006.png
│   │   │   ├── E007.png
│   │   │   ├── E008.png
│   │   │   ├── E305.png
│   │   │   ├── E306.png
│   │   │   ├── E307.png
│   │   │   ├── E308.png
│   │   │   ├── E405.png
│   │   │   ├── E406.png
│   │   │   ├── E407.png
│   │   │   ├── E408.png
│   │   │   ├── E505.png
│   │   │   ├── E506.png
│   │   │   ├── E507.png
│   │   │   └── E508.png
│   │   ├── output
│   │   └── PotentialField.ipynb                       # Script to plot the potential field of exciton
│   └── Graphs
│       ├── Coulomb_barridoB.ipynb                     # Coulomb binding energy calculate from comsol
│       ├── Datasets
│       │   └── m-1B0.txt
│       └── output
├── LICENSE                         # GPL-3.0 License
├── README.md                       # README.md
└── requirements.txt                # Python dependencies

```
