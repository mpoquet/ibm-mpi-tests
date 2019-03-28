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

main(int ac, char **av)
{
   int numtask,taskid,rc;
   int outmsg, inmsg, len = sizeof(outmsg); 
   int i,dest = 0, type = 1;
   int source, rtype = type, rbytes = -1,flag, dontcare = -1;
   int msgid;
   MPI_Status status;

   MPI_Init(0,0);
   MPI_Comm_size(MPI_COMM_WORLD,&numtask);
   MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
 
   if(taskid == 1) {
      MPI_Barrier(MPI_COMM_WORLD);
      outmsg = 5; type = 1;
      rc = MPI_Send(&outmsg, len,MPI_BYTE, dest, type,MPI_COMM_WORLD);
   }

   if(taskid == 0) {
      source = MPI_ANY_SOURCE; rtype = MPI_ANY_TAG;
      rc = MPI_Irecv(&inmsg,len,MPI_BYTE,source,rtype,MPI_COMM_WORLD,&msgid);
      rc = MPI_Test(&msgid,&flag,&status);
      if(flag)  printf("ERROR\n");
      MPI_Barrier(MPI_COMM_WORLD);
      rc = MPI_Wait(&msgid,&status);
      if(inmsg != 5 || status.MPI_SOURCE != 1 || status.MPI_TAG != 1)
         printf("ERROR\n");
   }
   MPI_Barrier(MPI_COMM_WORLD);
   if(taskid == 0)  printf("TEST COMPLETE\n");
   MPI_Finalize();
}
