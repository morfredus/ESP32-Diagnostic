#!/bin/bash
# MQTT Testing Script for ESP32 Diagnostic Suite
# Tests MQTT connectivity and metric publishing

set -e

# Configuration
ESP32_IP=${1:-"192.168.1.100"}
MQTT_BROKER=${2:-"broker.hivemq.com"}
MQTT_PORT=${3:-1883}
MQTT_PREFIX="esp32-diagnostic"

echo "╔════════════════════════════════════════════════════════════╗"
echo "║  ESP32 Diagnostic MQTT Testing Script                      ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""
echo "Configuration:"
echo "  ESP32 IP:      $ESP32_IP"
echo "  MQTT Broker:   $MQTT_BROKER:$MQTT_PORT"
echo "  Topic Prefix:  $MQTT_PREFIX"
echo ""

# Test 1: Check ESP32 Connectivity
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "TEST 1: Check ESP32 Web Server"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if curl -s "http://$ESP32_IP" > /dev/null 2>&1; then
    echo "✅ ESP32 web server responding"
else
    echo "❌ ERROR: Cannot reach ESP32 at $ESP32_IP"
    echo "   Make sure:"
    echo "   1. ESP32 is powered and WiFi connected"
    echo "   2. IP address is correct"
    echo "   3. ESP32 and your PC are on same network"
    exit 1
fi
echo ""

# Test 2: Check MQTT Configuration
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "TEST 2: Check MQTT Configuration"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

MQTT_STATUS=$(curl -s "http://$ESP32_IP/api/mqtt-status")
echo "MQTT Status Response:"
echo "$MQTT_STATUS" | jq '.' || echo "$MQTT_STATUS"

STATE=$(echo "$MQTT_STATUS" | jq -r '.state' 2>/dev/null || echo "error")

if [ "$STATE" = "connected" ]; then
    echo "✅ MQTT is connected"
elif [ "$STATE" = "connecting" ]; then
    echo "⏳ MQTT is connecting (wait 5 seconds and retry)"
elif [ "$STATE" = "disconnected" ]; then
    echo "❌ MQTT is disconnected"
    echo "   Troubleshooting:"
    echo "   1. Check config.h settings"
    echo "   2. Verify broker address and port"
    echo "   3. Check WiFi signal strength"
    echo "   4. Enable DIAGNOSTIC_VERBOSE in config.h for debug logs"
else
    echo "❌ MQTT is disabled or unknown state"
    echo "   Make sure ENABLE_MQTT_BRIDGE is true in config.h"
fi
echo ""

# Test 3: Check Broker Connectivity
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "TEST 3: Check Broker Connectivity"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if command -v nc &> /dev/null; then
    if nc -zv "$MQTT_BROKER" "$MQTT_PORT" 2>/dev/null; then
        echo "✅ Broker is reachable at $MQTT_BROKER:$MQTT_PORT"
    else
        echo "❌ Cannot reach broker at $MQTT_BROKER:$MQTT_PORT"
        echo "   Try testing with a different broker:"
        echo "   $0 $ESP32_IP broker.hivemq.com 1883"
    fi
else
    echo "⏭️  Skipping (nc not installed)"
fi
echo ""

# Test 4: Subscribe to Metrics
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "TEST 4: Subscribe to Metrics (30 seconds)"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if command -v mosquitto_sub &> /dev/null; then
    echo "Listening for MQTT messages from $MQTT_PREFIX/#"
    echo "(This will run for 30 seconds, press Ctrl+C to stop)"
    echo ""
    
    timeout 30s mosquitto_sub -h "$MQTT_BROKER" -p "$MQTT_PORT" \
        -t "$MQTT_PREFIX/#" -v --retained-only 2>/dev/null || true
    
    echo ""
    echo "✅ Received messages from broker"
    echo ""
else
    echo "⏭️  Skipping (mosquitto_sub not installed)"
    echo "   Install Mosquitto:"
    echo "   - macOS: brew install mosquitto"
    echo "   - Ubuntu: sudo apt install mosquitto-clients"
    echo "   - Docker: docker run -it eclipse-mosquitto"
    echo ""
fi

# Test 5: Publish Test Message
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "TEST 5: Publish Test Message"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

TEST_RESULT=$(curl -s "http://$ESP32_IP/api/mqtt-publish-test")
echo "Test Publish Response:"
echo "$TEST_RESULT" | jq '.' || echo "$TEST_RESULT"

if echo "$TEST_RESULT" | grep -q "success"; then
    echo "✅ Test message published"
else
    echo "❌ Test message publish failed"
fi
echo ""

# Test 6: Check Recent Metrics
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "TEST 6: Check Recent Metrics"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if command -v mosquitto_sub &> /dev/null; then
    echo "Fetching last published metrics..."
    METRICS=$(mosquitto_sub -h "$MQTT_BROKER" -p "$MQTT_PORT" \
        -t "$MQTT_PREFIX/#" --retained-only -W 2 2>/dev/null | head -20)
    
    echo "Recent metrics:"
    echo "$METRICS" | sed 's/^/  /'
    
    if [ -z "$METRICS" ]; then
        echo "  ⚠️  No metrics received (broker might not have retained messages)"
    fi
else
    echo "⏭️  Skipping (mosquitto_sub not installed)"
fi
echo ""

# Summary
echo "╔════════════════════════════════════════════════════════════╗"
echo "║  Testing Complete                                          ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""
echo "Next steps:"
echo "1. If all tests passed: MQTT is working! 🎉"
echo "2. If connection failed: Check broker address and WiFi"
echo "3. For Home Assistant: Add MQTT sensor in configuration.yaml"
echo "4. For NodeRED: Create MQTT In node"
echo "5. For monitoring: mosquitto_sub -h $MQTT_BROKER -t '$MQTT_PREFIX/#' -v"
echo ""
