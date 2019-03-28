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

#define ITER 10

main()
{
   int me,tasks,i,data[ITER],flag;
   MPI_Request request[ITER];
   MPI_Status status[ITER];

   MPI_Init(0,0);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);
   MPI_Comm_size(MPI_COMM_WORLD,&tasks);

   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  {
      for(i=0;i<ITER;i++)  {
         data[i] = i;
         MPI_Isend(&data[i],1,MPI_INT,1,1,MPI_COMM_WORLD,&request[i]);
         if(i == ITER/2)  
            MPI_Cancel(&request[i]);
      }
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Waitall(ITER,request,status);
      for(i=0;i<ITER;i++)  {
         MPI_Test_cancelled(&status[i],&flag);
         if(i==ITER/2 && !flag || i!= ITER/2 && flag)
            printf("task %d ERROR: Send request not cancelled! (2)\n",me);
      }
   } else if(me == 1) {
      MPI_Barrier(MPI_COMM_WORLD);
      for(i=0;i<ITER;i++)  {
         data[0] = -1;
         if(i != ITER/2)  {
            MPI_Recv(&data[0],1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
            if(data[0] != i)
                printf("ERROR: Incorrect data received (%d) i=%d\n",data[0],i);
         }
      }
   }
   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  printf("TEST COMPLETE\n");
   MPI_Finalize();
}
