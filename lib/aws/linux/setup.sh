#!/bin/sh

git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp.git

mkdir aws-sdk-cpp/build
cd aws-sdk-cpp/build

cmake .. -DENABLE_TESTING=OFF -DBUILD_ONLY="s3;ec2"

make
sudo make install
