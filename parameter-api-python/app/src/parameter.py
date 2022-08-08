"""
 * Copyright (C) 2022 Axis Communications AB, Lund, Sweden
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
"""

import sys
import grpc

# Protobuf packages generated from Dockerfile
import keyvaluestore_pb2
import keyvaluestore_pb2_grpc

target = "unix:///tmp/acap-runtime.sock"


def get_parameter_value(stub, request):
    response = stub.GetValues(request)
    return response.value


def main():
    if len(sys.argv) == 2:
        # gRPC secure connection
        root_cert = open(sys.argv[1], 'rb').read()
        ssl_creds = grpc.ssl_channel_credentials(root_cert)
        channel = grpc.secure_channel(target, ssl_creds)
    else:
        # gRPC insecure connection
        channel = grpc.insecure_channel(target)

    stub = keyvaluestore_pb2_grpc.KeyValueStoreStub(channel)

    param_names = [
        "root.Brand.Brand",
        "root.Brand.WebURL",
        "root.Image.I0.Enabled",
        "root.Brand.ProdFullName",
        "root.Brand.ProdNbr",
        "root.invalid"]

    for param_name in param_names:
        try:
            request = keyvaluestore_pb2.Request(key=param_name)
            param_value = get_parameter_value(stub, request)
            print("{} : {}".format(param_name, param_value))

        except grpc.RpcError as err:
            print("Error has occurred:", err.details)


if __name__ == '__main__':
    main()
