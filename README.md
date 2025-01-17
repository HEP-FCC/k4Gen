# k4Gen

Gaudi Components for Ge nerators in the Key4HEP software framework. See the
[FCC Tutorials](https://hep-fcc.github.io/fcc-tutorials/) for documentation on
the usage in FCC / Key4hep.

The pre-compiled components are available in the Key4hep stack on machines with
CVMFS:
```
source /cvmfs/sw.hsf.org/key4hep/setup.sh
```


## Dependencies

* Gaudi
* ROOT
* k4FWCore
* EDM4hep
* Pythia8
* EvtGen
* HepMC3
* HepPDT


## Building

After fetching the repository, do

```
source /cvmfs/sw.hsf.org/key4hep/setup.sh
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=install ..
make -j4
make install
cd ..
k4_local_repo  # will update all necessary environment variables
```
