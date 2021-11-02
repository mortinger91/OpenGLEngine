#include "Channel.h"

channel Channels::meshChan;
std::atomic<int> Channels::toRead = 0;