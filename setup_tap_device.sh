#!/bin/bash -e

if [[ $UID != 0 ]]; then
    echo "Please run this script with root permissions:"
    echo "sudo $0 $*"
    exit 1
fi

TAP_NODE=/dev/net/tap

# Create the TAP node.
mknod $TAP_NODE c 10 200
chmod 0666 $TAP_NODE

REAL_INTERFACE_NAME=ens33 
TAP_INTERFACE_NAME=tap0

# Setup firewall to NAT the traffic on the TAP interface to the ordinary
# interface, allowing jstack applicatons to access the real world.
iptables -I INPUT --source 10.0.0.0/24 -j ACCEPT
iptables -t nat -I POSTROUTING --out-interface $REAL_INTERFACE_NAME -j MASQUERADE
iptables -I FORWARD \
         --in-interface $REAL_INTERFACE_NAME \
         --out-interface $TAP_INTERFACE_NAME \
         -j ACCEPT
iptables -I FORWARD \
         --in-interface $TAP_INTERFACE_NAME \
         --out-interface $REAL_INTERFACE_NAME \
         -j ACCEPT
