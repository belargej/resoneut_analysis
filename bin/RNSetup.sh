export RN_ANALYSIS="$( cd "$( dirname "${BASH_SOURCE[0]}" )"&& cd .. && pwd )"

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${RN_ANALYSIS}/lib
export PATH=${PATH}:${RN_ANALYSIS}/bin