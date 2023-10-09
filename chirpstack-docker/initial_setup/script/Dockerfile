FROM python:3.9-slim
RUN pip install --upgrade pip
RUN pip install chirpstack-api==3.10.0
RUN pip install protobuf==3.20.1
RUN pip install pyyaml
COPY initial_setup.py /script/
CMD ["python", "/script/initial_setup.py", "--config", "/configuration/configuration.yaml"]