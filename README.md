# GRANN

This repository is a collection of ANNS algorithms to evaluate various metrics such as QPS vs Recall, # distance comparisons vs recall, etc. 

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

See [guidelines](CONTRIBUTING.md) for contributing to this project.



## Linux build:

Install the following packages through apt-get, and Intel MKL either by downloading the installer or using [apt](https://software.intel.com/en-us/articles/installing-intel-free-libs-and-python-apt-repo) (we tested with build 2019.4-070).
```
sudo apt install make cmake g++ libaio-dev libgoogle-perftools-dev clang-format libboost-dev
```
The version 4.0 of clang-format was not available, replac ing the command with the `sudo apt install clang-format` installs the version 10.0 and works well.

Installing Intel MKL on Linux
```
sudo apt install libmkl-full-dev
sudo apt-get install intel-mkl-full
```

Build
```
mkdir build && cd build && cmake .. && make -j 
```
If you face issues in the compilation that compiler not found install `build-essentials`
```
sudo apt-get update
sudo apt-get install -y build-essential
```


Intel mkl nneds to be installed for the build stage which can be installed by the instructions mentioned [here](https://www.intel.com/content/www/us/en/developer/tools/oneapi/onemkl-download.html?operatingsystem=linux&linux-install=apt)

The following command might be used alternatively
`sudo apt install intel-mkl`

Instllation of the IntelOneAPI it is needed as well, follow the steps [here](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit-download.html?operatingsystem=linux&linux-install-type=apt)

If you face issues with the above commands, use the `apt` package manager instead of the `apt-get` package manager
