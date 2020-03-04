/*
 * Configurable things.
 *
 * REMEMBER:
 * If you change CHANNELS or SIGNIFICANT_BITS, write the changes also to
 * JUDASCFG.INC. When finished with configuring, type wmake /a to completely
 * re-compile the JUDAS library.
 */

/*
 * How many times in second the DMA buffer goes round. The smaller this is,
 * the larger the buffer and the more there is delay in sound effects, but then
 * judas_update() doesn't have to be called so often. DMA buffer maximum size
 * (64K) sets a limit on the minimum value though. Don't fear, judas_init()
 * checks the buffer size and will simply limit it to 64K.
 */
#define PER_SECOND 20

/*
 * Number of digital channels. Increase or decrease according to your needs.
 * If it's below 32, all tunes can't be loaded. If it's above 32, you can
 * play 32-channel tunes and even have sound fx at the same time!
 * Number of channels will affect mixing speed just a tiny bit, however an
 * unused channel takes EXTREMELY little time compared to a used one.
 */
#define CHANNELS 32

/*
 * Number of significant bits in mixing. Maximum is 16. If it's below 16,
 * the leftover bits are used for clipping. At the current setting 14 2 bits
 * are used for clipping, so you can distort sound up to 4x without getting
 * an overflow (nasty-sounding distortion.) With 13 bits, you have 3 bits for
 * clipping and can distort up to 8x etc.
 */
#define SIGNIFICANT_BITS 14
