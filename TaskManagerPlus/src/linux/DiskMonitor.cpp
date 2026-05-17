#include "DiskMonitor.h"
#include "SystemStatus.h"
#include <cerrno>
#include <fstream>
#include <sys/statvfs.h>
#include <iostream>

void DiskMonitor::init(){
    m_prevSample = readSample();
}

void DiskMonitor::monitorLoop(){
    while (m_isRunning) {
        m_currentSample = readSample();
        computeDelta();
        {
            std::lock_guard<std::mutex> lock(systemStatus.diskMutex);
            systemStatus.checkQueueSize(systemStatus.diskTime);
            systemStatus.diskTime.push_front(m_diskUtil);
        }

        m_prevSample = m_currentSample;
    }
}

void DiskMonitor::computeDelta(){
    double deltaReadSectors = m_currentSample.sectorsRead - m_prevSample.sectorsRead; 
    double deltaWriteSectors = m_currentSample.sectorsWritten - m_prevSample.sectorsWritten;

    double bytesWrittenPerSecond = deltaWriteSectors * 512;
    double bytesReadPerSecond = deltaReadSectors * 512;

    systemStatus.writeDisk = (bytesWrittenPerSecond / 1024.0) / 1024.0;
    systemStatus.readDisk = (bytesReadPerSecond / 1024.0) / 1024.0;

    double deltaTime = m_currentSample.ioTime - m_prevSample.ioTime;
    m_diskUtil =  (deltaTime / 1000.0) * 100;
}

DiskMonitor::DiskSample DiskMonitor::readSample(){
    std::ifstream stat("/proc/diskstats");
    std::string line;

    while (std::getline(stat, line)) {
        if (line.find("nvme0n1") != std::string::npos) {
            continue;
        }
        break;
    }

    std::string majNumber, minNumber, deviceName;
    long successfulReads, mergedReads, sectorsRead, timeSpentReading, writesCompleted, writesMerged, sectorsWritten, timeSpentWriting, currentIO, timeSpentDoingIO;

    stat >> majNumber >> minNumber >> deviceName >> successfulReads >> mergedReads >> sectorsRead >> timeSpentReading >> writesCompleted >> writesMerged
        >> sectorsWritten >> timeSpentWriting >> currentIO >> timeSpentDoingIO;


    struct statvfs diskInfo;
    if (statvfs("/", &diskInfo) != 0) {
        std::cerr << errno;
    }

    unsigned long totalBytes = diskInfo.f_blocks * diskInfo.f_frsize;
    unsigned long availBytes = diskInfo.f_frsize * diskInfo.f_bavail;

    systemStatus.totalDisk = ((totalBytes / 1024.0) / 1024.0) / 1024.0;
    systemStatus.availDisk = ((availBytes / 1024.0) / 1024.0) / 1024.0;
    
    DiskSample sample;
    sample.sectorsRead = sectorsRead;
    sample.sectorsWritten = sectorsWritten;
    sample.ioTime = timeSpentDoingIO;

    return sample;
}
