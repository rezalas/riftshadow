#ifndef BITVECTOR_H
#define BITVECTOR_H

#define ARR(x) x < 32 ? 0 : 1

class CBitvector
{
public:
	CBitvector();
	~CBitvector();
	bool IsSet(int nBit);
	void SetBit(int nBit);
	void RemoveBit(int nBit);
	void ToggleBit(int nBit);
	void ClearBits(void);
	void SetToDouble(const char *rowptr);
	void SetToDouble(long long lval);
private:
	long bitvector[2];
};

#endif /* BITVECTOR_H */
