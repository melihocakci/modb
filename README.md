# MODB (Moving Object Database)

This project implements an abstraction layer for Berkeley DB and libspatialindex that is optimized for moving objects that change position frequently. We do this using a method called QU-trade.

## Building modb

    git submodule update --init --recursive
    ./pre_build.sh
    cmake .
    make

## GUIs
### Live Plane Api React.js
![Ekran görüntüsü 2023-06-12 025954](https://github.com/user-attachments/assets/32a1c7db-455e-4ab6-b7fd-5ce7413bccaa)

### Query With Plane Id
![Ekran görüntüsü 2023-06-11 183855](https://github.com/user-attachments/assets/e6ee1bcf-9547-4ede-a5b8-652464f1d600)


### Click and Query Interface
![Ekran görüntüsü 2023-06-10 200429](https://github.com/user-attachments/assets/3614bddd-ca0f-4cfa-b693-aa04f8fec7dc)


## Example Intersection query on Console
![Ekran görüntüsü 2023-06-11 161545](https://github.com/user-attachments/assets/0cf6cfe5-b24d-4289-b8e8-ea14067cb772)

## RTree-Btree Optimization
![Ekran görüntüsü 2023-06-04 164435](https://github.com/user-attachments/assets/135da8cb-cb1f-4b69-96a1-5c9c5920680e)
