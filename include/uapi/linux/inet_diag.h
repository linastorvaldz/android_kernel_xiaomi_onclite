#ifndef _UAPI_INET_DIAG_H_
#define _UAPI_INET_DIAG_H_

#include <linux/types.h>

/* Just some random number */
#define TCPDIAG_GETSOCK 18
#define DCCPDIAG_GETSOCK 19

#define INET_DIAG_GETSOCK_MAX 24

/* Socket identity */
struct inet_diag_sockid {
	__be16	idiag_sport;
	__be16	idiag_dport;
	__be32	idiag_src[4];
	__be32	idiag_dst[4];
	__u32	idiag_if;
	__u32	idiag_cookie[2];
#define INET_DIAG_NOCOOKIE (~0U)
};

/* Request structure */

struct inet_diag_req {
	__u8	idiag_family;		/* Family of addresses. */
	__u8	idiag_src_len;
	__u8	idiag_dst_len;
	__u8	idiag_ext;		/* Query extended information */

	struct inet_diag_sockid id;

	__u32	idiag_states;		/* States to dump */
	__u32	idiag_dbs;		/* Tables to dump (NI) */
};

struct inet_diag_req_v2 {
	__u8	sdiag_family;
	__u8	sdiag_protocol;
	__u8	idiag_ext;
	__u8	pad;
	__u32	idiag_states;
	struct inet_diag_sockid id;
};

enum {
	INET_DIAG_REQ_NONE,
	INET_DIAG_REQ_BYTECODE,
};

#define INET_DIAG_REQ_MAX INET_DIAG_REQ_BYTECODE

/* Bytecode is sequence of 4 byte commands followed by variable arguments.
 * All the commands identified by "code" are conditional jumps forward:
 * to offset cc+"yes" or to offset cc+"no". "yes" is supposed to be
 * length of the command and its arguments.
 */
 
struct inet_diag_bc_op {
	unsigned char	code;
	unsigned char	yes;
	unsigned short	no;
};

enum {
	INET_DIAG_BC_NOP,
	INET_DIAG_BC_JMP,
	INET_DIAG_BC_S_GE,
	INET_DIAG_BC_S_LE,
	INET_DIAG_BC_D_GE,
	INET_DIAG_BC_D_LE,
	INET_DIAG_BC_AUTO,
	INET_DIAG_BC_S_COND,
	INET_DIAG_BC_D_COND,
	INET_DIAG_BC_DEV_COND,   /* u32 ifindex */
	INET_DIAG_BC_MARK_COND,
};

struct inet_diag_hostcond {
	__u8	family;
	__u8	prefix_len;
	int	port;
	__be32	addr[0];
};

struct inet_diag_markcond {
	__u32 mark;
	__u32 mask;
};

/* Base info structure. It contains socket identity (addrs/ports/cookie)
 * and, alas, the information shown by netstat. */
struct inet_diag_msg {
	__u8	idiag_family;
	__u8	idiag_state;
	__u8	idiag_timer;
	__u8	idiag_retrans;

	struct inet_diag_sockid id;

	__u32	idiag_expires;
	__u32	idiag_rqueue;
	__u32	idiag_wqueue;
	__u32	idiag_uid;
	__u32	idiag_inode;
};

/* Extensions */

enum {
	INET_DIAG_NONE,
	INET_DIAG_MEMINFO,
	INET_DIAG_INFO,
	INET_DIAG_VEGASINFO,
	INET_DIAG_CONG,
	INET_DIAG_TOS,
	INET_DIAG_TCLASS,
	INET_DIAG_SKMEMINFO,
	INET_DIAG_SHUTDOWN,
	INET_DIAG_DCTCPINFO,
	INET_DIAG_PROTOCOL,  /* response attribute only */
	INET_DIAG_SKV6ONLY,
	INET_DIAG_LOCALS,
	INET_DIAG_PEERS,
	INET_DIAG_PAD,
	INET_DIAG_MARK,
	INET_DIAG_BBRINFO,
	INET_DIAG_C2TCPINFO,
	INET_DIAG_DEEPCCINFO,
	__INET_DIAG_MAX,
};

#define INET_DIAG_MAX (__INET_DIAG_MAX - 1)

/* INET_DIAG_MEM */

struct inet_diag_meminfo {
	__u32	idiag_rmem;
	__u32	idiag_wmem;
	__u32	idiag_fmem;
	__u32	idiag_tmem;
};

/* INET_DIAG_VEGASINFO */

struct tcpvegas_info {
	__u32	tcpv_enabled;
	__u32	tcpv_rttcnt;
	__u32	tcpv_rtt;
	__u32	tcpv_minrtt;
};

/* INET_DIAG_DCTCPINFO */

struct tcp_dctcp_info {
	__u16	dctcp_enabled;
	__u16	dctcp_ce_state;
	__u32	dctcp_alpha;
	__u32	dctcp_ab_ecn;
	__u32	dctcp_ab_tot;
};

/* INET_DIAG_BBRINFO */

struct tcp_bbr_info {
	/* u64 bw: max-filtered BW (app throughput) estimate in Byte per sec: */
	__u32	bbr_bw_lo;		/* lower 32 bits of bw */
	__u32	bbr_bw_hi;		/* upper 32 bits of bw */
	__u32	bbr_min_rtt;		/* min-filtered RTT in uSec */
	__u32	bbr_pacing_gain;	/* pacing gain shifted left 8 bits */
	__u32	bbr_cwnd_gain;		/* cwnd gain shifted left 8 bits */
	__u32	bbr_inflight;
};

/* INET_DIAG_C2TCPINFO */

struct tcp_c2tcp_info {
	__u32	c2tcp_min_rtt;		/* min-filtered RTT in uSec */
	__u32	c2tcp_avg_urtt;		/* averaged RTT in uSec from the previous info request till now*/
	__u32	c2tcp_cnt;		/* number of RTT samples used for averaging */
	__u64	c2tcp_avg_thr;		/* average throughput Bytes per Sec*/
	__u32	c2tcp_thr_cnt;		/* Number of sampled throughput for averaging it*/
};

/* INET_DIAG_DEEPCCINFO */
struct tcp_deepcc_info {
	__u32	min_rtt;		/* min-filtered RTT in uSec */
	__u32	avg_urtt;		/* averaged RTT in uSec from the previous info request till now*/
	__u32	cnt;		/* number of RTT samples used for averaging */
	__u64	avg_thr;		/* average throughput Bytes per Sec*/
	__u32	thr_cnt;		/* Number of sampled throughput for averaging it*/
	__u32	cwnd;
	__u32	pacing_rate;
	__u32	lost_bytes;			/* Number of lost Bytes (from the last monitored phase to now!)*/
	__u32	srtt_us;	/* smoothed round trip time << 3 in usecs */
	__u32	snd_ssthresh;	/* Slow start size threshold		*/
	__u32	packets_out;	/* Packets which are "in flight"	*/
	__u32	retrans_out;	/* Retransmitted packets out		*/
	__u32	max_packets_out;  /* max packets_out in last window */
	__u32 	mss_cache;
};

union tcp_cc_info {
	struct tcpvegas_info	vegas;
	struct tcp_dctcp_info	dctcp;
	struct tcp_bbr_info		bbr;
	struct tcp_c2tcp_info	c2tcp;
	struct tcp_deepcc_info	deepcc;
};
#endif /* _UAPI_INET_DIAG_H_ */
