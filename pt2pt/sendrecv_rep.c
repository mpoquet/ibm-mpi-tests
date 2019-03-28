/****************************************************************************

 MESSAGE PASSING INTERFACE TEST CASE SUITE

 Copyright IBM Corp. 1995

 IBM Corp. hereby grants a non-exclusive license to use, copy, modify, and
 distribute this software for any purpose and without fee provided that the
 above copyright notice and the following paragraphs appear in all copies.

 IBM Corp. makes no representation that the test cases comprising this
 suite are correct or are an accurate representation of any standard.

 In no event shall IBM be liable to any party for direct, indirect, special
 incidental, or consequential damage arising out of the use of this software
 even if IBM Corp. has been advised of the possibility of such damage.

 IBM CORP. SPECIFICALLY DISCLAIMS ANY WARRANTIES INCLUDING, BUT NOT LIMITED
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS AND IBM
 CORP. HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.

****************************************************************************

 These test cases reflect an interpretation of the MPI Standard.  They are
 are, in most cases, unit tests of specific MPI behaviors.  If a user of any
 test case from this set believes that the MPI Standard requires behavior
 different than that implied by the test case we would appreciate feedback.

 Comments may be sent to:
    Richard Treumann
    treumann@kgn.ibm.com

****************************************************************************
*/
#include "mpi.h"

main()
{
   int buf[1000],src,dest,sendtag,recvtag,tasks,me,i;
   MPI_Status status;

   MPI_Init(0,0);
   MPI_Comm_size(MPI_COMM_WORLD,&tasks);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);

   if(me < 2)  {
      src = dest = 1-me;
      sendtag = me;
      recvtag = src;
      for(i=0;i<100;i++)  buf[i] = me;
      MPI_Sendrecv_replace(buf,100,MPI_INT,dest,sendtag,src,recvtag,MPI_COMM_WORLD,&status);
      for(i=0;i<100;i++) 
         if(buf[i] != src)  printf("ERROR in MPI_Sendrecv: incorrect data\n");
      if(status.MPI_SOURCE != src)  printf("ERROR in MPI_Sendrecv: incorrect source\n");
      if(status.MPI_TAG != recvtag)  printf("ERROR in MPI_Sendrecv: incorrect tag\n");
   }

   src = (me==0) ? tasks-1 : me-1;
   dest = (me==tasks-1) ? 0 : me+1;
   sendtag = me;
   recvtag = src;
   for(i=0;i<100;i++) buf[i] = me; 
   MPI_Sendrecv_replace(buf,100,MPI_INT,dest,sendtag,src,recvtag,MPI_COMM_WORLD,&status);
   for(i=0;i<100;i++) 
      if(buf[i] != src)  printf("ERROR in MPI_Sendrecv: incorrect data\n");
   if(status.MPI_SOURCE != src)  printf("ERROR in MPI_Sendrecv: incorrect source\n");
   if(status.MPI_TAG != recvtag)  printf("ERROR in MPI_Sendrecv: incorrect tag\n");

   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  printf("TEST COMPLETE\n");
   MPI_Finalize();
}
