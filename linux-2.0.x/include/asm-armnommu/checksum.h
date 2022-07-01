#ifndef __ASM_ARM_CHECKSUM_H
#define __ASM_ARM_CHECKSUM_H

#ifndef __ASM_ARM_SEGMENT_H
#include <asm/segment.h>
#endif

/*
 * computes the checksum of a memory block at buff, length len,
 * and adds in "sum" (32-bit)
 *
 * returns a 32-bit number suitable for feeding into itself
 * or csum_tcpudp_magic
 *
 * this function must be called with even lengths, except
 * for the last fragment, which may be odd
 *
 * it's best to have buff aligned on a 32-bit boundary
 */
unsigned int csum_partial(const unsigned char * buff, int len, unsigned int sum);

/*
 * the same as csum_partial, but copies from src while it
 * checksums
 *
 * here even more important to align src and dst on a 32-bit (or even
 * better 64-bit) boundary
 */

unsigned int csum_partial_copy( const char *src, char *dst, int len, int sum);


/*
 * the same as csum_partial_copy, but copies from user space.
 *
 * here even more important to align src and dst on a 32-bit (or even
 * better 64-bit) boundary
 */

static __INLINE__ unsigned int csum_partial_copy_fromuser(const char *src, char *dst, int len, int sum)
{
	extern unsigned int __csum_partial_copy_fromuser(const char *src, char *dst, int len, int sum);

	if (IS_USER_SEG)
		return __csum_partial_copy_fromuser (src, dst, len, sum);
	else
		return csum_partial_copy (src, dst, len, sum);
}

/*
 *	This is a version of ip_compute_csum() optimized for IP headers,
 *	which always checksum on 4 octet boundaries.
 *
 *	By Jorge Cwik <jorge@laser.satlink.net>, adapted for linux by
 *	Arnt Gulbrandsen.
 *
 *	Converted to ARM by R.M.King
 */
static inline unsigned short ip_fast_csum(unsigned char * iph,
					  unsigned int ihl) {
	unsigned int sum, tmp1;

    __asm__ __volatile__("
	sub	%2, %2, #5
	ldr	%0, [%1], #4
	ldr	%3, [%1], #4
	adds	%0, %0, %3
	ldr	%3, [%1], #4
	adcs	%0, %0, %3
	ldr	%3, [%1], #4
	adcs	%0, %0, %3
1:	ldr	%3, [%1], #4
	adcs	%0, %0, %3
	tst	%2, #15
	subne	%2, %2, #1
	bne	1b
	adc	%0, %0, #0
	adds	%0, %0, %0, lsl #16
	addcs	%0, %0, #0x10000
	mvn	%0, %0
	mov	%0, %0, lsr #16
	 "
	: "=&r" (sum), "=&r" (iph), "=&r" (ihl), "=&r" (tmp1)
	: "1" (iph), "2" (ihl));
	return(sum);
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented
 */
static inline unsigned short int csum_tcpudp_magic(unsigned long saddr,
						   unsigned long daddr,
						   unsigned short len,
						   unsigned short proto,
						   unsigned int sum) {
    __asm__ __volatile__("
    adds	%0, %0, %1
    adcs	%0, %0, %4
    adcs	%0, %0, %5
    adc		%0, %0, #0
    adds	%0, %0, %0, lsl #16
    addcs	%0, %0, #0x10000
    mvn		%0, %0
    mov		%0, %0, lsr #16
	"
	: "=&r" (sum), "=&r" (saddr)
	: "0" (daddr), "1"(saddr), "r"((ntohs(len)<<16)+proto*256), "r"(sum));
	return((unsigned short)sum);
}

/*
 * 	Fold a partial checksum without adding pseudo headers
 */
static inline unsigned int csum_fold(unsigned int sum)
{
    __asm__ __volatile__("
    adds	%0, %0, %0, lsl #16
    addcss	%0, %0, #0x10000
    addcs	%0, %0, #0x10000
    mvn		%0, %0
    mov		%0, %0, lsr #16
	"
	: "=r" (sum)
	: "0" (sum));
	return sum;
}	
	

/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */

static inline unsigned short ip_compute_csum(unsigned char * buff, int len) {
    unsigned int sum;

    __asm__ __volatile__("
    adds	%0, %0, %0, lsl #16
    addcs	%0, %0, #0x10000
    mvn		%0, %0
    mov		%0, %0, lsr #16
	"
	: "=r"(sum)
	: "0" (csum_partial(buff, len, 0)));
	return(sum);
}

#endif
