#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

// Function to read frames from the text file
std::vector<std::vector<std::string>> readFramesFromFile(const std::string &filename)
{
  std::vector<std::vector<std::string>> frames;
  std::ifstream file(filename);
  std::string byte;
  while (file >> byte)
  {
    if (frames.empty() || frames.back().size() == 8)
    {
      frames.emplace_back();
    }
    frames.back().push_back(byte);
  }
  return frames;
}

// Function to configure and open the serial port
HANDLE openSerialPort(const std::string &portName)
{
  HANDLE hSerial = CreateFile(portName.c_str(),
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              0,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              0);

  if (hSerial == INVALID_HANDLE_VALUE)
  {
    std::cerr << "Error opening serial port" << std::endl;
    return INVALID_HANDLE_VALUE;
  }

  DCB dcbSerialParams = {0};
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

  if (!GetCommState(hSerial, &dcbSerialParams))
  {
    std::cerr << "Error getting serial port state" << std::endl;
    CloseHandle(hSerial);
    return INVALID_HANDLE_VALUE;
  }

  dcbSerialParams.BaudRate = CBR_9600;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;

  if (!SetCommState(hSerial, &dcbSerialParams))
  {
    std::cerr << "Error setting serial port state" << std::endl;
    CloseHandle(hSerial);
    return INVALID_HANDLE_VALUE;
  }

  COMMTIMEOUTS timeouts = {0};
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;

  if (!SetCommTimeouts(hSerial, &timeouts))
  {
    std::cerr << "Error setting serial port timeouts" << std::endl;
    CloseHandle(hSerial);
    return INVALID_HANDLE_VALUE;
  }

  return hSerial;
}

// Function to send data to the Arduino
void sendData(HANDLE hSerial, const std::string &filename, int delayMs)
{
  while (true)
  {
    auto frames = readFramesFromFile(filename);

    if (frames.empty())
    {
      std::cerr << "Failed to read frames or frames.txt is empty" << std::endl;
      return;
    }

    for (const auto &frame : frames)
    {
      std::string frameData;
      for (const auto &byte : frame)
      {
        frameData += byte + " ";
      }
      frameData.pop_back(); // Remove the trailing space
      frameData += "\n";

      DWORD bytesWritten;
      WriteFile(hSerial, frameData.c_str(), frameData.size(), &bytesWritten, NULL);

      // Print the data being sent
      std::cout << "Sent: " << frameData;

      Sleep(delayMs);
    }
  }
}

int main()
{
  std::string filename = "frames.txt";
  std::string portName = "\\\\.\\COM3"; // COM3 port
  int delayMs = 1500;                   // Adjust the delay as needed

  HANDLE hSerial = openSerialPort(portName);
  if (hSerial == INVALID_HANDLE_VALUE)
  {
    return 1;
  }

  sendData(hSerial, filename, delayMs);

  CloseHandle(hSerial);
  return 0;
}
