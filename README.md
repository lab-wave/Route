# Фреймворк для моделирования и построения динамических расписаний движения больших групп агентов (в том числе автономных) в мультиагентных транспортных и логистических системах высокой плотности 
![image](https://github.com/user-attachments/assets/decbcd53-5c54-444c-a314-4185e7d59a16)

Фреймворк «Маршрут» представляет собой комплекс программных инструментов для ускоренного построения систем планирования и оптимизации траекторий движения групп автономных агентов (движущихся объектов различного типа), а также диспетчеризации потока заданий в транспортных и логистических мультиагентных системах в условиях высокой плотности. Под мультиагентной системой понимается группа интеллектуальных агентов (в том числе автономных), которые взаимодействуют друг с другом для решения задач, выходящих за рамки индивидуальных возможностей или знаний каждого агента. Каждый агент в такой системе обладает собственными задачами и сценарием функционирования, а также способен взаимодействовать с другими агентами для достижения как индивидуальных, так и коллективных задач. Такие системы используются в задачах, где действия нескольких агентов должны быть скоординированы, они могут быть кооперативными, конкурентными или сочетать в себе и то, и другое. Тематика соответствует ВТН «Искусственный интеллект» и направлениям реализации Программы создания и развития Центра НТИ (проект «Создание Фреймворка для моделирования и построения динамических расписаний движения больших групп агентов (в том числе автономных) в мультиагентных транспортных и логистических системах высокой плотности»).

# Установка
Установка не предполагается

# Сборка
Используется система сборки CMake.

# Описание Прототипа Фреймворка "Маршрут" 
На данном этапе реализована основная архитектура Фреймворка «Маршрут», включающая:
- интерфейсы для работы с графами - ребрами и вершинами. Вершины графа представляют собой узлы, через которые перемещаются агенты, а ребра — пути между ними. Для каждого ребра могут быть заданы следующие параметры: тип ребра (двунаправленное или однонаправленное), вершины начала и окончания ребра, значение веса ребра для алгоритмов построения пути, набор атрибутов ребра. Для вершин могут быть заданы следующие параметры: уникальный идентификатор вершины, набор смежных ребер (входящих и исходящих), функции добавления ребер, набор атрибутов вершины.
- интерфейсы класса графа состояний агентов и класса агента для моделирования и работы с агентами. Класс графа состояний агентов содержит: набор вершин и ребер графа, функции диапазонов с приведением к необходимому классу и фильтрацией для ребер и вершин, набор атрибутов графа. Реализован интерфейс класса состояния агента, которая содержит функцию сравнения по времени события.
- интерфейс для класса моделирования поиска пути агентом и класс контейнера дерева, который содержит алгоритмы: поиск в ширину по потомкам заданной вершины, обход всех родителей заданной вершины, выделение всех листов дерева для заданной вершины.
- интерфейс класса симулятора, который содержит: функцию создания экземпляра графа для моделирования по описанию, функцию создания агента для моделирования по его описанию, функцию запуска моделирования с учетом оптимизации движения, функцию остановки моделирования, функцию возврата статистических параметров моделирования.
- для описания всех основных сущностей, используемых при моделировании, реализован класс описания атрибутов и их ограничений, позволяющий получать эти данные из файла формата xml. 
- класс статистических параметров результатов моделирования. Статистика результатов моделирования включает: временную статистику (значения статистик, вычисленные на некоторый момент времени моделирования), пространственную статистику с привязкой к моделируемому графу (значения статистик, вычисленные на момент окончания моделирования, данные могут использоваться для визуализации «тепловых карт»), общие статистики (значения статистик вычисленные на момент окончания моделирования или не зависящие от времени моделирования, могут включать слепок входных параметров).
- 
В состав прототипа Фреймворка «Маршрут» включен прототип модуля Искусственного интеллекта (МИИ), реализующий построение расписания движения агентов с учетом выбора транзитных узлов для минимизации суммарной длины маршрута. Результатом работы прототипа МИИ являются расписание перевозок с учетом транзитных узлов. Алгоритм работы основан на методах кластерного анализа и включает применение подходов машинного обучения для динамического выделения групп терминалов и оптимизации маршрутов агентов в условиях изменяющихся параметров сети. 
Входными данными для прототипа МИИ из состава прототипа Фреймворка «Маршрут» являются:
- данные о транзитных узлах (название, свойство транзитности);
- данные о терминалах (идентификатор, название, адрес, свойство транзитности);
- данные о перевозках (идентификатор, отправитель, получатель, дата формирования задачи, характеристики перевозки). 
Входные данные представлены в формате csv.

# Благодарности
Работа выполнена лабораторией волновых процессов и систем управления Центра компетенций НТИ на базе МФТИ по направлению "Искусственный интеллект" в рамках "дорожной карты" развития высокотехнологичного направления "Искусственный интеллект" на период до 2030 года при поддержке Фонда НТИ.

# Лицензия
Распространяется по лицензии BSD 3-Clause.
