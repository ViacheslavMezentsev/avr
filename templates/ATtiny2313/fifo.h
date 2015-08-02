#pragma once

// Простое FIFO: http://mainloop.ru/c-language/simple-fifo.html

// Update:
// Чтобы использовать этот код для fifo c размером 256 и более,
// надо для head и tail поменять тип c unsigned char на unsigned short.

// Размер должен быть степенью двойки: 4,8,16,32...128.
#define FIFO( size )\
  struct {\
    unsigned char tail;\
    unsigned char head;\
    unsigned char buf[size];\
  }

// Количество элементов в очереди.
#define FIFO_COUNT(fifo)     (fifo.head-fifo.tail)

// Размер fifo.
#define FIFO_SIZE(fifo)      ( sizeof(fifo.buf)/sizeof(fifo.buf[0]) )

// fifo заполнено?
#define FIFO_IS_FULL(fifo)   (FIFO_COUNT(fifo)==FIFO_SIZE(fifo))

// fifo пусто?
#define FIFO_IS_EMPTY(fifo)  (fifo.tail==fifo.head)

// Количество свободного места в fifo.
#define FIFO_SPACE(fifo)     (FIFO_SIZE(fifo)-FIFO_COUNT(fifo))

// Поместить элемент в fifo.
#define FIFO_PUSH(fifo, byte) \
  {\
    fifo.buf[fifo.head & (FIFO_SIZE(fifo)-1)]=byte;\
    fifo.head++;\
  }

// Взять первый элемент из fifo.
#define FIFO_FRONT(fifo) (fifo.buf[fifo.tail & (FIFO_SIZE(fifo)-1)])

// Уменьшить количество элементов в очереди.
#define FIFO_POP(fifo)   \
  {\
      fifo.tail++; \
  }

// Очистить fifo.
#define FIFO_FLUSH(fifo)   \
  {\
    fifo.tail=0;\
    fifo.head=0;\
  }
