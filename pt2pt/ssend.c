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
   int buf[10],len,tasks,me,i;
   double time; 
   MPI_Status status;

   len = sizeof(buf); 
   MPI_Init(0,0);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);
   MPI_Barrier(MPI_COMM_WORLD);
   if(me==0) {
      MPI_Ssend(buf,len,MPI_CHAR,1,1,MPI_COMM_WORLD); 
      time = MPI_Wtime();
      MPI_Send(&time,1,MPI_DOUBLE,1,2,MPI_COMM_WORLD);
   } else if(me == 1) {
      for(i=0;i<3000000;i++) ;
      MPI_Recv(buf,len,MPI_CHAR,0,1,MPI_COMM_WORLD,&status);
      MPI_Recv(&time,1,MPI_DOUBLE,0,2,MPI_COMM_WORLD,&status);
      time = time - MPI_Wtime();
      if(time < 0) time = -time;
      if(time > .1) printf("ERROR: MPI_Ssend did not synchronize\n"); 
   }
   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  printf("TEST COMPLETE\n");
   MPI_Finalize();
}
