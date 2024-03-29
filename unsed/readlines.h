#include <stddef.h>
// Структура, в которой прячется файловый дескриптор, буфер и хозяйственная
// информация.
typedef struct {
    int fd;
    size_t max_size;
    char *buf;
    int size;
    int skip;
} RL;

// Принимает файловый дескриптор и максимальный размер строки, которую требуется
// обрабатывать и возвращает указатель на свежевыделенную структуру типа RL
// или NULL в случае ошибки.
RL* rl_open(int fd, size_t max_size);

// Возвращает max_size, указанный при создании rl.
size_t rl_max_size (RL *rl);

// Освобождает структуру rl. При этом следует закрыть и файловый дескриптор,
// спрятанный внутри. Возвращает $0$, если всё хорошо или $-1$ в случае ошибки.
// В последнем случае код ошибки должен возвращаться через errno.
int rl_close(RL *rl);

// Читает строку (включая символ перевода строки) размером не более
// $min(max_size + 1, buf_size)$ из файлового дескриптора спрятанного
// в rl в буфер buf.
// * В случае успеха записывает строку в buf (включая перевод строки) и возвращает
// длину записанных в buf данных. Частичный результат не допускается.
// * В случае конца ввода возвращает $0$.
// * В случае, если строка в файловом дескрипторе больше `max_size + 1`
// возвращает $-3$. Несколько последующих вызовов также могут возвращать $-3$
// (пропуск хвоста слишком длинной строки).
// * В случае, если длина строки на вводе меньше `max_size + 1`, но больше
// `buf_size`, то возвращает $-2$. Строка при этом безвозвратно выбрасывается.
// * В остальных случаях возвращает $-1$, устанавливая errno в код произошедшей
// ошибки.
// Философские факты: бывают строки нулевой длины, все строки файла кроме
// последней всегда заканчиваются `\n`, последняя строка в файле никогда не
// заканчивается `\n`.
int rl_readline(RL *rl, char * buf, size_t buf_size);
