#!/bin/bash
docker build -t "edbt24:DockerEDBT24" .
docker run -it "edbt24:DockerEDBT24" bash
