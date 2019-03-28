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

int main()
{
   int myself,tasks;
   MPI_Status status;
 
   int ii;
   int numtasks, me;
   int len;
   int count1,count2,count3;
   MPI_Aint disp;
   MPI_Datatype type,newtype;

   MPI_Init(0,0);
   MPI_Comm_rank(MPI_COMM_WORLD,&myself);

   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &me);

   if ((numtasks < 2)) {
     printf("this testcase requires 2 tasks.\n");
     MPI_Abort(MPI_COMM_WORLD, me);
   }
    
   if ((numtasks > 2) && (me > 1)) { 
#ifdef DB_TALK
     printf("Testcase uses two tasks, extraneous task #%d exited.\n",me);
#endif
     MPI_Finalize(); 
     exit(1);
   }
   len = 1;
   type = MPI_INT;
   MPI_Type_struct(0,&len,&disp,&type,&newtype);
   MPI_Type_commit(&newtype);

   if(myself == 0)  {
      ii = 2;
      MPI_Send(&ii,100,newtype,1,0,MPI_COMM_WORLD);
   } else if(myself == 1) {
      ii = 0;
      MPI_Recv(&ii,100,newtype,0,0,MPI_COMM_WORLD,&status);
      if(ii != 0)
         printf("ERROR!\n");
      MPI_Get_count(&status,newtype,&count1);
      MPI_Get_elements(&status,newtype,&count2);
      MPI_Get_count(&status,MPI_BYTE,&count3);
      if ( (count1==MPI_UNDEFINED) &&
           (count2==MPI_UNDEFINED) &&
           (count3==0)             ) 
        printf("Success\n");
      else
        printf("Should be -1, -1, 0 but is %d, %d, %d,\n",
               count1,count2,count3);
   } 
}
