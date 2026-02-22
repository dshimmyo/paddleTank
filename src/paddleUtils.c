// Function to pack hue, saturation, and luminosity into an 8-bit value
unsigned char packColor(unsigned char h,unsigned char s, unsigned char v) {
    return (h & 0x07) << 5 | (s & 0x03) << 3 | (v & 0x07);
}