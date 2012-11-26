#pragma once

// Размер должен быть степенью двойки: 4,8,16,32...256
#define FIFO( size )\
    struct {\
        unsigned char buf[ size ];\
        unsigned char tail;\
        unsigned char head;\
    }

// fifo заполнено?
#define FIFO_IS_FULL(fifo)   ((fifo.head-fifo.tail)==sizeof(fifo.buf))

// fifo пусто?
#define FIFO_IS_EMPTY(fifo)  (fifo.tail==fifo.head)

// Количество элементов в очереди
#define FIFO_COUNT(fifo)     (fifo.head-fifo.tail)

// Размер fifo
#define FIFO_SIZE(fifo)      ( (sizeof(fifo.buf)))

// Количество свободного места в fifo
#define FIFO_SPACE(fifo)     (FIFO_SIZE(fifo)-FIFO_COUNT(fifo))

// Поместить элемент в fifo
#define FIFO_PUSH(fifo, byte) \
  {\
    fifo.buf[fifo.head & (sizeof(fifo.buf)-1)]=byte;\
    fifo.head++;\
  }

// Взять первый элемент из fifo
#define FIFO_FRONT(fifo) (fifo.buf[(fifo).tail & (sizeof(fifo.buf)-1)])

// Уменьшить количество элементов в очереди
#define FIFO_POP(fifo)   \
  {\
      fifo.tail++; \
  }

// Очистить fifo
#define FIFO_FLUSH(fifo)   \
  {\
    fifo.tail=0;\
    fifo.head=0;\
  }
