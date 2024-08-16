В репозитории представлены [однопоточный](/single-threaded-mergesort/src/) и [многопоточный](/multi-threaded-mergesort/src/) алгоритмы сортировки слиянием для последовательностей. Однопоточная версия является основой для распараллеливания. 

Для выполнения параллельных задач реализован планировщик [thread_pool](/multi-threaded-mergesort/src/sched/scheduler.hpp).