#ifndef PARSER_H
#define PARSER_H

#include <thread>
#include <iostream>
#include "EventQueue.h"
#include "Device.h"

class Parser
{
public:
  /*!
   * \brief Конструктор
   *
   * \param queue Умный указатель на очередь.
   * \param A Умный указатель на устройство A.
   * \param B Умный указатель на устройство B.
  */
  Parser(std::shared_ptr<EventQueue> queue, std::shared_ptr<Device> A, std::shared_ptr<Device> B) : queue_(queue), A_(A), B_(B) {};

  /*!
   * \brief Функция, запускающая чтение данных с устройств в параллельном режиме.
   *
   * \param loop_count_A Беззнаковое целое число, задающее кол-во итераций чтения с устройства А.
   * \param loop_count_B Беззнаковое целое число, задающее кол-во итераций чтения с устройства В.
   * \param crush_index_A Целое число, указывающая на какой итерации сломается устройство А.
   * \param crush_index_B Целое число, указывающая на какой итерации сломается устройство B.
  */
  void run(size_t loop_count_A, size_t loop_count_B, int crush_index_A = -1, int crush_index_B = -1) {
    std::thread threadA(&Parser::read, this, A_, std::chrono::seconds(1), loop_count_A, crush_index_A);
    std::thread threadB(&Parser::read, this, B_, std::chrono::seconds(5), loop_count_A, crush_index_A);

    threadA.join();
    threadB.join();

    while (true) {
      auto event = queue_->pop(std::chrono::seconds(5));

      if (event) {
        event->toString();
      }
      else {
        std::cout << "No events received for 5 seconds. Exiting...\n";
        break;
      }
    }
  };

private:
  /*!
   * \brief Читает данные с устройства.
   *
   * \param device Умный указатель на устройство
   * \param sleep_duration Секунды, задающие длительность прерывания для симулированной работы.
   * \param loop_count Беззнаковое целое число, задающее кол-во итераций чтения с устройства.
   * \param crush_index Целое число, указывающая на какой итерации сломается устройство.
  */
  void read(std::shared_ptr<Device> device, std::chrono::seconds sleep_duration, size_t loop_count, int crush_index);

private:
  std::shared_ptr<EventQueue> queue_;
  std::shared_ptr<Device> A_;
  std::shared_ptr<Device> B_;
};

#endif
