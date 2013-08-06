#pragma once

// ������ ������ ���� �������� ������: 4,8,16,32...256
#define FIFO( size )\
    struct {\
        unsigned char buf[ size ];\
        unsigned char tail;\
        unsigned char head;\
    }

// fifo ���������?
#define FIFO_IS_FULL(fifo)   ((fifo.head-fifo.tail)==sizeof(fifo.buf))

// fifo �����?
#define FIFO_IS_EMPTY(fifo)  (fifo.tail==fifo.head)

// ���������� ��������� � �������
#define FIFO_COUNT(fifo)     (fifo.head-fifo.tail)

// ������ fifo
#define FIFO_SIZE(fifo)      ( (sizeof(fifo.buf)))

// ���������� ���������� ����� � fifo
#define FIFO_SPACE(fifo)     (FIFO_SIZE(fifo)-FIFO_COUNT(fifo))

// ��������� ������� � fifo
#define FIFO_PUSH(fifo, byte) \
  {\
    fifo.buf[fifo.head & (sizeof(fifo.buf)-1)]=byte;\
    fifo.head++;\
  }

// ����� ������ ������� �� fifo
#define FIFO_FRONT(fifo) (fifo.buf[(fifo).tail & (sizeof(fifo.buf)-1)])

// ��������� ���������� ��������� � �������
#define FIFO_POP(fifo)   \
  {\
      fifo.tail++; \
  }

// �������� fifo
#define FIFO_FLUSH(fifo)   \
  {\
    fifo.tail=0;\
    fifo.head=0;\
  }
