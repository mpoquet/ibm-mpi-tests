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
   int me,tasks,i,data,flag;
   MPI_Request request;
   MPI_Status status;

   MPI_Init(0,0);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);
   MPI_Comm_size(MPI_COMM_WORLD,&tasks);

   if(me == 0)  {
      MPI_Irecv(&data,1,MPI_INT,1,1,MPI_COMM_WORLD,&request);
      MPI_Cancel(&request);
      MPI_Wait(&request,&status);
      MPI_Test_cancelled(&status,&flag);
      if(!flag) printf("task %d ERROR: Receive request not cancelled!\n",me);
      MPI_Issend(&data,100000,MPI_INT,1,1,MPI_COMM_WORLD,&request);
      MPI_Cancel(&request);
      for(flag=0;;)  {
         MPI_Test(&request,&flag,&status);
         if(flag) break;
      }
      MPI_Test_cancelled(&status,&flag);
      if(!flag) printf("task %d ERROR: Send request not cancelled! (1)\n",me);
   }

   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  {
      data = 5;
      MPI_Isend(&data,1,MPI_INT,1,1,MPI_COMM_WORLD,&request);
      MPI_Cancel(&request);
      MPI_Wait(&request,&status);
      MPI_Test_cancelled(&status,&flag);
      if(!flag) printf("task %d ERROR: Send request not cancelled! (2)\n",me);
      data = 6;
      MPI_Send(&data,1,MPI_INT,1,5,MPI_COMM_WORLD);

      MPI_Isend(&data,1,MPI_INT,1,1,MPI_COMM_WORLD,&request);
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Cancel(&request);
      MPI_Wait(&request,&status);
      MPI_Test_cancelled(&status,&flag);
      if(flag) printf("task %d ERROR: Send request cancelled!\n",me);
   } else if(me == 1) {
      for(i=0;i<1000000;i++);
      data = 0;
      MPI_Recv(&data,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
      if(data != 6) printf("task %d ERROR: Send request not cancelled!\n",me);

      MPI_Recv(&data,1,MPI_INT,0,5,MPI_COMM_WORLD,&status);
      MPI_Barrier(MPI_COMM_WORLD);
   }
   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  printf("TEST COMPLETE\n");
   MPI_Finalize();
}
