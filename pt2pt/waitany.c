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
   int me,tasks,i,req[2000],data[2000],index;
   MPI_Status status;

   MPI_Init(0,0);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);
   MPI_Comm_size(MPI_COMM_WORLD,&tasks);

   if(me) 
      MPI_Send(&me,1,MPI_INT,0,1,MPI_COMM_WORLD);
   else {
      req[0] = MPI_REQUEST_NULL;
      for(i=1;i<tasks;i++)  
         MPI_Irecv(&data[i],1,MPI_INT,i,1,MPI_COMM_WORLD,&req[i]);

      MPI_Waitany(0,req,&index,&status);  
      if(index != MPI_UNDEFINED)
         printf("ERROR in MPI_Waitany: index not = MPI_UNDEFINED\n");

      for(i=1;i<tasks;i++)  {
         MPI_Waitany(tasks,req,&index,&status);  
         if(index == MPI_UNDEFINED)
            printf("ERROR in MPI_Waitany: index = MPI_UNDEFINED\n");
         if(req[index] != MPI_REQUEST_NULL)
            printf("ERROR in MPI_Waitany: reqest not set to NULL\n");
         if(data[index] != index)
            printf("ERROR in MPI_Waitany: wrong data\n");
      }

      MPI_Waitany(tasks,req,&index,&status);  
      if(index != MPI_UNDEFINED)
         printf("ERROR in MPI_Waitany: index not = MPI_UNDEFINED\n");
   }
   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  printf("TEST COMPLETE\n");
   MPI_Finalize();
}
