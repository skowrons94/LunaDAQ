# Description

LunaDAQ interface for data acquisition based on the DAQ written at LNL.

# Requirements

The following ones can be installed from package manager, eg. on Ubuntu:

```bash
sudo apt install cmake libgsoap-dev libboost-all-devel qt5-default mesa-common-dev libtinyxml2-dev libcurl4-openssl-dev libjsoncpp-dev
```

The CAEN drivers (in order of installation: CAENVMElib CAENComm CAENDigitizers)
must be downloaded from their website and installed following their 
instructions. If the board is connect through the USB, also CAENUSBdrv is 
needed.

A working ROOT installation is needed. Maybe it can be required to change the 
C++ standard in the CMakeLists.txt to the one with which ROOT was compiled, 
otherwise compatibilty errors could arise.

# Build / Installation

To build the repository:

```bash
git clone https://baltig.infn.it/LUNA_DAQ/LunaDAQ.git
cd LunaDAQ && mkdir build && cd build/
cmake ..
make && sudo make install
```

The binaries will be installed in ```/opt/LunaDAQ/```. You can add the ```export PATH=/opt/LunaDAQ/${PATH}``` line to your ```bashrc``` file to start it from 
terminal or create a symlink to you ```bin``` directory.

To run properly the LunaDAQ it is necessary to install the DAQ based on XDAQ 
environment (write skowrons@pd.infn.it for more details). In alternative, the 
docker container on https://hub.docker.com/repository/docker/skowrons/xdaq can 
be used instead and contains all the codes already builded and installed.

# Features

The software is able both to control the XDAQ instances and read/write the 
CAEN boards. The predefined ```.xml``` file used by XDAQ is located in 
```/opt/LunaDAQ/conf/``` and it is copied in each Project directory. Thus, 
it might be necessary to change it accordingly to where the XDAQ libraries 
are located and accordingly to the hostname of the PC.

The XDAQ instances are started in separate ```tmux``` instances, thus it 
is possible to attach to them if needed. 

LocalFilter instance additionally provide the optional feature to send data 
statistics to an existing ```graphite``` server, which can be added during 
the project initialization. Learn more at https://graphiteapp.org/ website.
The ```graphite``` database can be sourced from ```grafana``` dashboard 
making it useful for statistics visualization.

The LunaDAQ permits to spy on a running acquistion by visualizing both the 
energy/charge histograms and the waveforms. It is also possible to convert 
the ReadoutUnit and LocalFilter files into ROOT format directly from the GUI.

Not all the boards are fully supported. Here is the list of the board families 
which are fully supported: XX730, XX725, XX720, XX724, XX781, XX782. Both the 
PHA and PSD supported. For now, there is no compatibility with the WaveCatcher 
firmware, but can be added if need.

# Bugs / To Do

If any bug arises, pleas write to jakub.skowronski@pd.infn.it

- Add the ELOG interface.
- Add the REST interface for the accelerator.
- Add analysis tools in the HistoSpyInterface and WaveSpyInterface.
