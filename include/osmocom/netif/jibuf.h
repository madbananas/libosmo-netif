#ifndef _JIBUF_H_
#define _JIBUF_H_

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <osmocom/core/timer.h>

/*! \defgroup jibuf Osmocom Jitter Buffer
 *  @{
 */

/*! \file jibuf.h
 *  \brief Osmocom Jitter Buffer helpers
 */

typedef void (*osmo_jibuf_dequeue_cb)(struct msgb *msg, void *data);

/*! \brief A structure representing a single instance of a jitter buffer */
struct jibuf {
	void *talloc_ctx;
	bool started;
	struct osmo_timer_list timer;
	struct llist_head msg_list; /* sorted by output ts */
	uint32_t min_delay; /* in msec */
	uint32_t max_delay; /* in msec */
	uint32_t threshold_delay; /* in msec */

	osmo_jibuf_dequeue_cb dequeue_cb;
	void *dequeue_cb_data;

	/* number of pkt drops since we last changed the buffer size */
	uint32_t last_dropped;
	uint32_t consecutive_drops;

	uint32_t ref_rx_ts;
	uint32_t ref_tx_ts;
	uint16_t ref_tx_seq;

	struct timeval last_enqueue_time;
	struct timeval next_dequeue_time;

	struct {
		uint32_t total_enqueued;
		uint64_t total_dropped;
	} stats;
};


struct jibuf *osmo_jibuf_alloc(void *talloc_ctx);

void osmo_jibuf_delete(struct jibuf *jb);

int osmo_jibuf_enqueue(struct jibuf *jb, struct msgb *msg);

bool osmo_jibuf_empty(struct jibuf *jb);

void osmo_jibuf_set_min_delay(struct jibuf *jb, uint32_t min_delay);
void osmo_jibuf_set_max_delay(struct jibuf *jb, uint32_t max_delay);

void osmo_jibuf_set_dequeue_cb(struct jibuf *jb, osmo_jibuf_dequeue_cb dequeue_cb, void* cb_data);

/*! @} */

#endif
