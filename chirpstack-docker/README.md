# Overview

This repository contains a Docker configuration for standing up [Chirpstack](https://www.chirpstack.io/) to support the DDRN demo, as well as utilities and instructions for setting it up on a new PC.

See the component diagram below for the default static IP addresses and ports for a [Lorix One](https://www.lorixone.io/en) gateway deployment.

![Diagram of component connections](/doc_images/HardwareLayout.svg "Component Diagram")

# Steps

## Install Docker for Windows

See instructions at https://docs.docker.com/docker-for-windows/install/. Using the WSL 2 backend is ideal.

## Clone this Repository

If necessary, download and install Git for Windows from https://git-scm.com/downloads.

Open a shell and run the following to download a copy of this repository:

```shell
git clone https://gitlab.asynchrony.com/proj-1728/chirpstack-docker
```

Enter the newly created directory:

```shell
cd chirpstack-docker
```

## Start Chirpstack

Enter the chirpstack subdirectory and start the stack with:

```shell
cd chirpstack
docker compose up -d
```

Docker will first download the necessary images and then start each service in the background.

Confirm that the stack is up and running by navigating to http://localhost:8090/. A login screen should appear.

## Create a Chirpstack API Token

Navigate to http://localhost:8090 and log in with admin / admin if necessary. Then navigate to http://localhost:8090/#/api-keys.

Click "Create" and enter any name for the token.

![Screenshot of Chirpstack Web UI with API key creation screen for name field](/doc_images/api_token_name.png "API Key Creation Screen - Name Field")

Click "Create API Key" to display the generated key. Save this value for use in the next step.

![Screenshot of Chirpstack Web UI with API key creation screen for generated token value](/doc_images/api_token_value.png "API Key Creation Screen - Token Value")

## Run Setup Script

Return to the shell and change directory:

```shell
cd ../initial_setup
```

Open an File Explorer showing the configuration file with:

```shell
start configuration
```

Open the ```configuration.yaml``` file in a text editor and replace the "api_token" value with the key created in the previous step. Save and close the file.

Back in the terminal, run the setup script via Docker with:

```shell
docker compose build
docker compose up
```

The script will exit after completion. The output should look similar to:

```shell
Container initial_setup_initial-setup_1  Creating
Container initial_setup_initial-setup_1  Created
Attaching to initial-setup_1
initial-setup_1  | Created network server with id 1
initial-setup_1  | Created service profile with id ff0740eb-be54-4fa5-b22c-b20709d76a6f
initial-setup_1  | Created gateway with id fcc23dfffe0b9f67
initial-setup_1  | Created gateway with id ac1f09fffe0144e0
initial-setup_1  | Created application with id 1
initial-setup_1  | Created HTTP integration
initial-setup_1  | Created device profile with name DDRN Node
initial-setup_1 exited with code 0
```

## Verify Gateway Connectivity

The PC's Ethernet adaptor should be set to use the static IP 192.168.1.2. If this does not meet deployment requirements, the gateway can be reconfigured to support another address.

Connect the Lorix One gateway to the PC an a power supply using the included POE injector.

Navigate to http://localhost:8090/#/organizations/1/gateways/fcc23dfffe0b9f67 and refresh occasionally. The Last Seen At field should update once the gateway has reported its own status to Chirpstack. This may take a few minutes.

![Screenshot of Chirpstack Web UI with gateway overview screen](/doc_images/gateway_last_seen_at.png "Gateway Overview Screen")

## Register a DDRN Node

LoRaWAN devices can now be registered with the network server. We first need to find the Device EUI identifier for the reporting DDRN node.

Navigate to http://localhost:8090/#/organizations/1/gateways/fcc23dfffe0b9f67/frames and power on the DDRN node. A message should be logged within a few seconds as the device attempts to join the LoRaWAN network. Take note of the DevEUI:

![Screenshot of Chirpstack Web UI with gateway messages screen](/doc_images/device_join.png "Gateway Messages Screen")

Navigate to http://localhost:8090/#/organizations/1/applications/1/devices/create to register this device. Enter the Device EUI copied earlier and enter other fields as shown here:

![Screenshot of Chirpstack Web UI with first device registration screen](/doc_images/device_registration_1.png "Device Registration Screen")

Click "Create Device", and then enter the application key ```cff6b2f4d0d9c0e6779b32b06b6029a1``` on the next screen:

![Screenshot of Chirpstack Web UI with second device registration screen](/doc_images/device_registration_2.png "Device Registration Screen")

A summary screen for the create device will appear. Click the "Device Data" tab to observe received messages:

![Screenshot of Chirpstack Web UI with device overview screen](/doc_images/device_data.png "Device Overview Screen")

Entries will appear as the device transmits LoRaWAN messages. If nothing appears, unpower and repower the device to observe initial join messages. Error messages may occur periodically and can generally be ignored. This occur in normal operation and should resolve automatically.

![Screenshot of Chirpstack Web UI with device data overview screen](/doc_images/device_messages.png "Device Data Screen")

## Verify Integration with MFK

If necessary, install MFK-CBRN using the provided installers and instruction manual.

Start MFK and navigate to the dashboard. The gateway and added device should appear and be updated a the device sends LoRaWAN messages.

![Screenshot of MFK daskboard tool showing gateway and device entriess](/doc_images/mfk.png "MFK-CBRN Dashboard")

## Configure MFK to Issue Commands

Click the Preferences button in MFK and select "ChirpStack" from the left menu.

![Screenshot of MFK ChirpStack preferences](/doc_images/mfk_chirpstack_prefs.png "MFK-CBRN ChirpStack Preferences")

Enter the ChirpStack Host, Port, and API Key generated previously. Typically, host will be "localhost" and port will be "8090". Then click the OK button.

![Screenshot of MFK ChirpStack preferences](/doc_images/mfk_chirpstack_prefs_populated.png "MFK-CBRN ChirpStack Preferences Populated")

Click the drop-down arrow next to your ChirpStack sensor in the MFK dashboard. A list of commands supported by the sensor will be displayed. Click a command to send it to the sensor.

![Screenshot of MFK Sensor Commands List](/doc_images/mfk_commands_list.png "MFK-CBRN Sensor Commands List")