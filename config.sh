#!/bin/bash

echo "==================== SYSTEM INFORMATION ===================="

echo -e "\n--- CPU INFO ---"
# Model i freq base (aprox)
lscpu | grep -E 'Model name|CPU MHz|Socket|Core\(s\) per socket|Thread'

# Nombre de cores físics i lògics
PHYSICAL_CORES=$(lscpu | awk '/Core\(s\) per socket/ {cores=$4} /Socket\(s\)/ {sockets=$2} END {print cores*sockets}')
LOGICAL_CORES=$(lscpu | awk '/^CPU\(s\)/ {print $2}')

echo "Physical cores: $PHYSICAL_CORES"
echo "Logical cores:  $LOGICAL_CORES"

echo -e "\n--- MEMORY INFO ---"
# Memòria total
MEM_TOTAL=$(free -h | awk '/Mem:/ {print $2}')
echo "Total RAM: $MEM_TOTAL"

# Type of memory
if command -v dmidecode &> /dev/null; then
    echo "Memory type:"
    sudo dmidecode -t memory | grep -E "Type:|Speed:" | grep -v "Unknown" | head -n 4
else
    echo "Memory type: (requires 'sudo dmidecode')" #I didn't have sudo permisions,so I couldn't try this line, could be wrong"
fi

echo -e "\n--- CACHE INFO ---"
# L1, L2, L3 (if they're available
lscpu | grep -E 'L1d cache|L1i cache|L2 cache|L3 cache'

echo -e "\n--- OPERATING SYSTEM ---"
echo "OS:"
lsb_release -a 2>/dev/null || cat /etc/os-release

echo "============================================================"
