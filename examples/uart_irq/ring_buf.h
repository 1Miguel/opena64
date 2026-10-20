/*******************************************************************************
 * @file ring_buf.h
 * @brief A simple ring buffer module.
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#ifndef RING_BUF__H
#define RING_BUF__H

#include <stdint.h>
#include <string.h>
#include <errno.h>

struct ring_buf {
  void *buf;      /*!< pointer to buffer array */
  uint16_t len;   /*!< len of the buffer array */
  uint16_t size;  /*!< size of an object in buffer */
  uint16_t count; /*!< item count */ 
  uint16_t head;  /*!< head pointer */ 
  uint16_t tail;  /*!< tail pointer */ 
};

/**
 * @brief static ring buffer initialiser.
 *
 * @param _buf  buffer to register.
 * @param _len  length of the buffer to register.
 * @param _size per element size.
 */
#define RING_BUF_INIT(_buf, _len, _size) { \
  .buf = _buf, \
  .len = _len, \
  .size = _size, \
  .count = 0, \
  .head = 0, \
  .tail = 0, \
}

int ring_buf_reset(struct ring_buf *rb)
{
  if (!rb)
    return -EINVAL;

  rb->count = 0;
  rb->head = 0;
  rb->tail = 0;
  return 0;
}

int ring_buf_put(struct ring_buf *rb, void *item, uint16_t len)
{
  uint16_t n = 0;

  if (!rb || !item || !len)
    return -EINVAL;

  if (rb->count == rb->len)
    return -EAGAIN;

  while (n <= len) {
    memcpy((rb->buf + rb->tail), (item + n), rb->size);

    n++;
    rb->tail++;
    rb->count++;

    if (rb->count >= rb->len)
      break;
  }

  return n;
}

int ring_buf_get(struct ring_buf *rb, void *item, uint16_t len)
{
  uint16_t n = 0;

  if (!rb || !item || !len)
    return -EINVAL;

  if (!rb->count)
    return -EAGAIN;

  while (n <= len) {
    memcpy((item + n), (rb->buf + rb->head), rb->size);

    n++;
    rb->head++;
    rb->count--;

    if (rb->count >= rb->len)
      break;
  }

  return n;
}

#endif // RING_BUF__H
