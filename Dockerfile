FROM ubuntu:22.04
LABEL maintainer="bergamigiacomo@gmail.com"

ENV DEBIAN_FRONTEND noninteractive

# APT Update
RUN apt-get --allow-releaseinfo-change update
RUN apt-get update && apt-get upgrade -y

RUN apt-get install -y build-essential automake autoconf cmake curl wget git g++-11 gcc-11
RUN mkdir /root/edbt24
ADD . /root/edbt24
RUN mkdir /root/edbt24/build
RUN cd /root/edbt24/build && cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles" -DCMAKE_MAKE_PROGRAM=make CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ .. && make mining_tests until_tests complete_response basic_operators_testing knobab2-edbt24-benchmarking && ./mining_tests && ./until_tests && ./complete_response && ./basic_operators_testing
CMD ["bash"]
