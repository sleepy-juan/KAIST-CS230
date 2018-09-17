void mult_int_32_to_64(unsigned x, unsigned y, unsigned *xyh, unsigned *xyl)
{
	// initializing the results pointer
    *xyh = 0;
    *xyl = 0;

    // determine sign bit
    *xyh = ((x >> 31) ^ (y >> 31)) << 31;

    // two's complement if negative
    x = (x ^ (-(x >> 31))) + (x >> 31);
    y = (y ^ (-(y >> 31))) + (y >> 31);

    // addition
    unsigned i = 0;
    for(i = 0; i<32; i++){
        if((x >> i) & 1){
            if(i != 0) // for positive i (since right shift 32 is equal to y itself not 0)
                *xyh = (*xyh) + (y >> (32 - i));
            if((*xyl) > (*xyl) + (y << i)) // overflow
                *xyh = (*xyh) + 1;
            *xyl = (*xyl) + (y << i);
        }
    }

    // two's complement if negative
    *xyl = (*xyl ^ (-(*xyh >> 31))) + (*xyh >> 31);
    *xyh = (*xyh ^ (-(*xyh >> 31))) + ((*xyh >> 31) << 31);
    if(!(*xyl) && ((*xyh) >> 31)) // overflow
        (*xyh)++;
}

void float_add(unsigned x, unsigned y, unsigned *result)
{
	// clear pointer
	*result = 0;

	// divide components
	unsigned sign_x = (0x80000000 & x);
	unsigned exponent_x = (0x7f800000 & x) >> 23;
	unsigned mantissa_x = (0x007fffff & x) | 0x800000; // IEEE 754

	unsigned sign_y = (0x80000000 & y);
	unsigned exponent_y = (0x7f800000 & y) >> 23;
	unsigned mantissa_y = (0x007fffff & y) | 0x800000; // IEEE 754

    unsigned shifted_mantissa = 0; // storing shifted mantissa.

	// exponent_x <- result exponent
	if(exponent_y > exponent_x){
        shifted_mantissa = mantissa_x << (32 - exponent_y + exponent_x);
		mantissa_x = mantissa_x >> (exponent_y - exponent_x);
		exponent_x = exponent_y;
	} else {
        shifted_mantissa = mantissa_y << (32 - exponent_x + exponent_y);
        if(exponent_x == exponent_y)
            shifted_mantissa = 0; // handle if shifting 32 bits.

		mantissa_y = mantissa_y >> (exponent_x - exponent_y);
		exponent_y = exponent_x;
	}

	// we can assume exponent_x containing result exponent from here
	// addition (only mantissa handling in this if-condition)
	if(sign_x ^ sign_y){ // different sign
		// sign_x <- result sign
		if(sign_x){ // y - x
			if(mantissa_x > mantissa_y){ // result sign is negative
				*result = mantissa_x - mantissa_y;
			}else{
				*result = mantissa_y - mantissa_x;
				sign_x = 0;
			}
		} else { // x - y
			if(mantissa_x > mantissa_y)
				*result = mantissa_x - mantissa_y;
			else{
				*result = mantissa_y - mantissa_x;
				sign_x = 0x80000000;
			}
		}

        shifted_mantissa = ~shifted_mantissa + 1; // two's complement.

        if(shifted_mantissa != 0)
            *result = (*result) - 1; // 1 takes for subtracting shifted mantissa

		if((*result) == 0){ // case of zero
			*result = 0;
			exponent_x = 0;
			sign_x = 0;
            shifted_mantissa = 0;
		} else {
			// we can assume sign_x containing result sign from here
			while(!((*result) & 0x800000)){ // case of underflow
				*result = (*result) << 1;
                *result = (*result) | (shifted_mantissa >> 31);
                shifted_mantissa = shifted_mantissa << 1;
				exponent_x--;
                if(exponent_x == 0)
                    break;
			}
            if(exponent_x == 0){ // case of denomalized
                *result = 0;
                exponent_x = 0;
                sign_x = 0;
                shifted_mantissa = 0;
            }
		}
	}
	else { // same sign
		*result = mantissa_x + mantissa_y;
		if((*result) & 0x1000000){ // overflow
			exponent_x++;
            shifted_mantissa = shifted_mantissa >> 1;
            shifted_mantissa = shifted_mantissa | (((*result) & 1) << 31);
			*result = (*result) >> 1;
		}
	}

    //rounding
    /*
    if(shifted_mantissa & 0x80000000){ // else, do nothing
        if(shifted_mantissa & 0x40000000){ // 11x
            *result = (*result) + 1; // rounding up
        } else { // 10x
            if(shifted_mantissa & 0x20000000){ // 101
                *result = (*result) + 1;
            } else { // tie to even
                if((*result) & 1)
                    *result = (*result) + 1;
            }
        }
    }
    */

    //rounding
    if(shifted_mantissa > 0x80000000){ // else, do nothing
        *result = (*result) + 1;
    } else if(shifted_mantissa == 0x80000000){
        if((*result) & 1){
            *result = (*result) + 1;
        }
    }    

	// make result
	*result = (*result) & 0x007fffff;
	*result = (*result) | (exponent_x << 23);
	*result = (*result) | sign_x;

    // zero handling
    if(x == 0)
        *result = y;
    if(y == 0)
        *result = x;
}

void float_mult(unsigned x, unsigned y, unsigned *result)
{
	// clear result pointer
    *result = 0;

    // set sign bit
    *result = ((x >> 31) ^ (y >> 31)) << 31;

    // set exponent
    unsigned exponent = (x & 0x7f800000) + (y & 0x7f800000) - 0x3f800000;
    if(exponent & 0x80000000) // overflow
        ; // not handled in this assignment;

    unsigned mantissa_x = (0x007fffff & x) | 0x800000; // IEEE 754
    unsigned mantissa_y = (0x007fffff & y) | 0x800000; // IEEE 754

    unsigned xyh =0, xyl = 0;
    mult_int_32_to_64(mantissa_x, mantissa_y, &xyh, &xyl);

    // shift until xyh fits mantissa
    xyh = xyh << 8;
    xyh = xyh | (xyl >> 24);
    xyl = xyl << 8;
    if(xyh & 0x800000) // overflow
        exponent += 0x800000;
    else{
        xyh = xyh << 1;
        xyh = xyh | (xyl >> 31);
        xyl = xyl << 1;
    }

    // rounding
    /*
    if(xyl & 0x80000000){ // else, do nothing
        if(!(xyl & 0x60000000)){ // tie
            if(xyl & 1){ // round up
                xyh++;
                if(xyh & 0x1000000){ // overflow
                    exponent++;
                    xyh = xyh >> 1;
                }  
            }
        } else { // round up
            xyh++;
            if(xyh & 0x1000000){ // overflow
                exponent++;
                xyh = xyh >> 1;
            }
        }
    }
    */

    // rounding
    // for some cases that I made
    if(xyl > 0x80000000){ // else, do nothing
        xyh++;
        if(xyh & 0x1000000){
            exponent++;
            xyh = xyh >> 1;
        }
    } else if(xyl == 0x80000000){
        if(xyh&1){
            xyh++;
            if(xyh & 0x1000000){
                exponent++;
                xyh = xyh >> 1;
            }   
        }
    }

    // make result
    *result = (*result) | xyh;
    *result = (*result) & 0x807fffff;
    *result = (*result) | exponent;

    // zero handling
    if(x == 0 || y == 0)
        *result = 0;
}
