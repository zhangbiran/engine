#include "ACE_Message_Block_Test.h"
#include <iostream>
using namespace std;
#include <assert.h>
#include <ace/OS.h>
#include <ace/Message_Block.h>
#include <ace/OS_NS_unistd.h>
#include <ace/ACE.h>

#pragma comment(lib, "aced.lib")

void ACE_Message_Block_Test::test(int argc, char **argv)
{
	cout << "ACE_Message_Block_Test::test" << endl;
	ACE_Message_Block *head = new ACE_Message_Block(BUFSIZ);
	ACE_Message_Block *mblk = head;

	for (;;) {
		ssize_t nbytes = ACE::read_n(ACE_STDIN,
			mblk->wr_ptr(),
			mblk->size());
		if (nbytes <= 0)
			break; // Break out at EOF or error.

		// Advance the write pointer to the end of the buffer.
		mblk->wr_ptr(nbytes);

		// Allocate message block and chain it at the end of list.
		mblk->cont(new ACE_Message_Block(BUFSIZ));
		mblk = mblk->cont();
	}
	// Print the contents of the list to the standard output.
	for (mblk = head; mblk != 0; mblk = mblk->cont())
		ACE::write_n(ACE_STDOUT, mblk->rd_ptr(), mblk->length());

	head->release(); // This releases all the memory in the chain.
}