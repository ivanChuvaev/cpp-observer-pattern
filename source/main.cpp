#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <map>
#include "ObserverPattern.h"

void redirectStdout () {
  freopen("logs/logfile.txt", "w", stdout);
}

std::string fileToString(std::string filename) {
  std::ifstream stream(filename);
  std::streampos fileSize = stream.tellg();
  stream.seekg(0, std::ios::end);
  fileSize = stream.tellg() - fileSize;
  stream.seekg(0, std::ios::beg);
  char *buffer = (char *)malloc((unsigned long)fileSize + 1);
  buffer[fileSize] = '\0';
  stream.read(buffer, fileSize);
  stream.close();
  std::string out = buffer;
  return out;
}

class FileObserver : public ObserverHandler<std::string>
{
  std::string filename;
  std::string fileContent;
  std::thread *thread;
  bool shouldExit;

  static const std::map<std::string, int> commands;

public:
  void update()
  {
    std::string newFileContent = fileToString(this->filename);

    if (newFileContent.length() != 0)
    {
      this->notify(newFileContent);
      this->fileContent = newFileContent;
      std::ofstream stream(this->filename);
      stream.close();
    }
  }

  void loopUpdate()
  {
    while (true)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
      this->update();
      if (this->shouldExit)
      {
        break;
      }
    }
  }

  FileObserver(std::string filename)
  {
    this->filename = filename;
    this->shouldExit = false;
    FILE *file = fopen(this->filename.c_str(), "r");
    if (file == nullptr)
    {
      throw std::runtime_error(filename + " does not exist");
    }
    fclose(file);
    this->thread = nullptr;
  }

  void watch()
  {
    this->shouldExit = false;
    this->thread = new std::thread(&FileObserver::loopUpdate, this);
  }

  void stop()
  {
    this->shouldExit = true;
  }

  void join()
  {
    if (this->thread != nullptr)
    {
      this->thread->join();
    }
  }
};

void handleFileCommand (Observer<std::string>* fobserver) {
  enum Command {
    exit,
    unsupported,
    get_id,
    clear_log,
    draw_pict1,
    draw_pict2
  };

  static Command (*translateCommand)(std::string) = [](std::string str) -> Command {
    static std::map<std::string, Command> commandMap = {
      {"exit", Command::exit},
      {"get_id", Command::get_id},
      {"clear_log", Command::clear_log},
      {"draw_pict1", Command::draw_pict1},
      {"draw_pict2", Command::draw_pict2}
    };
    try {
      return commandMap.at(str);
    } catch(std::out_of_range err) {
      return Command::unsupported;
    }
  };

  auto observerCallback = [fobserver](std::string content) {
    Command command = translateCommand(content);
    switch (command)
    {
      case exit:
        // observer will affect it's handler
        std::cout<<&fobserver<<" exited\n";
        ((FileObserver*)fobserver->getHandler())->stop();
        break;
      case Command::get_id:
        std::cout<<"id: "<<fobserver->getHandler()<<std::endl;
        break;
      case Command::clear_log:
        redirectStdout();
        break;
      case Command::draw_pict1:
        std::cout<<fileToString("./ascii_picts/pict1.txt")<<std::endl;
        break;
      case Command::draw_pict2:
        std::cout<<fileToString("./ascii_picts/pict2.txt")<<std::endl;
        break;
      default:
        std::cout<<"msg from "<<fobserver->getHandler()<<": "<<content<<std::endl;
        break;
    }
  };

  fobserver->addCallback(observerCallback, &observerCallback);
}

int main()
{
  redirectStdout();
  setbuf(stdout, NULL); // disable buffering for stdout so printf will flush instantly

  FileObserver first("input_files/luigi.txt");
  FileObserver second("input_files/mario.txt");
  first.watch();
  second.watch();

  Observer<std::string> observerForFirst;
  observerForFirst.subscribe(&first);
  handleFileCommand(&observerForFirst);

  Observer<std::string> observerForSecond;
  observerForSecond.subscribe(&second);
  handleFileCommand(&observerForSecond);

  first.join();
  second.join();
  return 0;
}