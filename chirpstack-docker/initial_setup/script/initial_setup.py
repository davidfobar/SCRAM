import argparse
import grpc
from chirpstack_api.as_pb.external import api
import yaml


def find_network_server(channel, auth_token, matching_server):
    client = api.NetworkServerServiceStub(channel)

    req = api.ListNetworkServerRequest()
    req.limit = 100
    req.offset = 0

    resp = client.List(req, metadata=auth_token)

    for network_server in resp.result:
        if network_server.server == matching_server:
            return network_server.id

    return None


def create_network_server(channel, auth_token, name, server):
    client = api.NetworkServerServiceStub(channel)

    req = api.CreateNetworkServerRequest()
    req.network_server.name = name
    req.network_server.server = server

    resp = client.Create(req, metadata=auth_token)

    return resp.id


def find_service_profile(channel, auth_token, matching_network_server_id):
    client = api.ServiceProfileServiceStub(channel)

    req = api.ListServiceProfileRequest()
    req.limit = 100
    req.offset = 0
    req.organization_id = 1
    req.network_server_id = matching_network_server_id

    resp = client.List(req, metadata=auth_token)

    for service_profile in resp.result:
        if service_profile.network_server_id == matching_network_server_id:
            return service_profile.id

    return None


def create_service_profile(channel, auth_token, name, network_server_id):
    client = api.ServiceProfileServiceStub(channel)

    req = api.CreateServiceProfileRequest()
    req.service_profile.name = name
    req.service_profile.organization_id = 1
    req.service_profile.network_server_id = network_server_id
    req.service_profile.add_gw_metadata = True

    resp = client.Create(req, metadata=auth_token)

    return resp.id


def find_gateway(channel, auth_token, matching_id):
    client = api.GatewayServiceStub(channel)

    req = api.ListGatewayRequest()
    req.limit = 100
    req.offset = 0
    req.organization_id = 1

    resp = client.List(req, metadata=auth_token)

    for gateway in resp.result:
        if gateway.id == matching_id:
            return True

    return False


def create_gateway(channel, auth_token, id, name, network_server_id, service_profile_id):
    client = api.GatewayServiceStub(channel)

    req = api.CreateGatewayRequest()
    req.gateway.id = id
    req.gateway.name = name
    req.gateway.organization_id = 1
    req.gateway.network_server_id = network_server_id
    req.gateway.service_profile_id = service_profile_id
    req.gateway.location.latitude = 0
    req.gateway.location.longitude = 0

    client.Create(req, metadata=auth_token)


def find_application(channel, auth_token, matching_name):
    client = api.ApplicationServiceStub(channel)

    req = api.ListApplicationRequest()
    req.limit = 100
    req.offset = 0
    req.organization_id = 1

    resp = client.List(req, metadata=auth_token)

    for application in resp.result:
        if application.name == matching_name:
            return application.id

    return None


def create_application(channel, auth_token, name, service_profile_id):
    client = api.ApplicationServiceStub(channel)

    req = api.CreateApplicationRequest()
    req.application.name = name
    req.application.organization_id = 1
    req.application.service_profile_id = service_profile_id

    resp = client.Create(req, metadata=auth_token)

    return resp.id


def find_http_integration(channel, auth_token, application_id, matching_event_endpoint_url):
    client = api.ApplicationServiceStub(channel)

    req = api.GetHTTPIntegrationRequest()
    req.application_id = application_id

    try:
        client.GetHTTPIntegration(req, metadata=auth_token)
        return True
    except:
        return False


def create_http_integration(channel, auth_token, application_id, event_endpoint_url):
    client = api.ApplicationServiceStub(channel)

    req = api.CreateHTTPIntegrationRequest()
    req.integration.application_id = application_id
    req.integration.event_endpoint_url = event_endpoint_url 
    req.integration.marshaler = api.Marshaler.JSON

    client.CreateHTTPIntegration(req, metadata=auth_token)


def find_device_profile(channel, auth_token, matching_name):
    client = api.DeviceProfileServiceStub(channel)

    req = api.ListDeviceProfileRequest()
    req.limit = 100
    req.offset = 0
    req.organization_id = 1

    resp = client.List(req, metadata=auth_token)

    for device_profile in resp.result:
        if device_profile.name == matching_name:
            return device_profile.id

    return None


def create_device_profile(channel, auth_token, network_server_id, device_profile_name, device_profile_mac_version, device_profile_reg_params_revision, device_profile_supports_join):
    client = api.DeviceProfileServiceStub(channel)

    req = api.CreateDeviceProfileRequest()
    req.device_profile.name = device_profile_name
    req.device_profile.organization_id = 1
    req.device_profile.network_server_id = network_server_id
    req.device_profile.mac_version = device_profile_mac_version
    req.device_profile.reg_params_revision = device_profile_reg_params_revision
    req.device_profile.supports_join = device_profile_supports_join

    client.Create(req, metadata=auth_token)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Chirpstack initial setup script"
    )
    parser.add_argument("--config", help="configuration file in YAML format")
    args = parser.parse_args()

    with open(args.config) as f:
        configuration = yaml.load(f, Loader=yaml.FullLoader)
    
    api_token = configuration['api_token']
    server = configuration['server']
    auth_token = [("authorization", "Bearer %s" % api_token)]
    channel = grpc.insecure_channel(server)

    # Create network server

    network_server_name = configuration['network_server']['name']
    network_server_url = configuration['network_server']['url']

    network_server_id = find_network_server(channel, auth_token, network_server_url)
    if network_server_id is not None:
        print('Network server already exists with id %d' % network_server_id)
    else:
        network_server_id = create_network_server(channel, auth_token, network_server_name, network_server_url)
        print('Created network server with id %d' % network_server_id)

    # Create service profile

    service_profile_name = configuration['service_profile']['name']

    service_profile_id = find_service_profile(channel, auth_token, network_server_id)
    if service_profile_id is not None:
        print('Service profile already exists with id %s' % service_profile_id)
    else:
        service_profile_id = create_service_profile(channel, auth_token, service_profile_name, network_server_id)
        print('Created service profile with id %s' % service_profile_id)

    # Create gateways

    for gateway in configuration['gateways']:
        gateway_id = gateway['id']
        gateway_name = gateway['name']

        if find_gateway(channel, auth_token, gateway_id):
            print('Gateway already exists with id %s' % gateway_id)
        else:
            create_gateway(channel, auth_token, gateway_id, gateway_name, network_server_id, service_profile_id)
            print('Created gateway with id %s' % gateway_id)

    # Create application

    application_name = configuration['application']['name']

    application_id = find_application(channel, auth_token, application_name)
    if application_id is not None:
        print('Application already exists with id %s' % application_id)
    else:
        application_id = create_application(channel, auth_token, application_name, service_profile_id)
        print('Created application with id %s' % application_id)

    # Create HTTP integration

    event_endpoint_url = configuration['http_integration']['url']

    if find_http_integration(channel, auth_token, application_id, event_endpoint_url):
        print('HTTP integration already exists')
    else:
        create_http_integration(channel, auth_token, application_id, event_endpoint_url)
        print('Created HTTP integration')

    # Create device profiles

    for device_profile in configuration['device_profiles']:
        device_profile_name = device_profile['name']
        device_profile_mac_version = device_profile['mac_version']
        device_profile_reg_params_revision = device_profile['reg_params_revision']
        device_profile_supports_join = device_profile['supports_join']

        if find_device_profile(channel, auth_token, device_profile_name):
            print('Device profile already exists with name %s' % device_profile_name)
        else:
            create_device_profile(channel, auth_token, network_server_id, device_profile_name, device_profile_mac_version, device_profile_reg_params_revision, device_profile_supports_join)
            print('Created device profile with name %s' % device_profile_name)