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
/* This testcase knowingly violates the rule which requires sender and    
   receiver type signatures to match.  This is done to provide situations
   which can test some other functions.                                   */
#include "mpi.h"

int main( argc, argv )
int argc;
char **argv;
{
    char imessage[1000], omessage[1000];
    char packbuf[1000];
    int i,j,k,count;
    int error = 0;
    int numtasks,me,other;
    MPI_Aint iaddr, iaddr1, iaddr2, iaddr3;
    MPI_Aint oaddr, oaddr1, oaddr2, oaddr3;
    MPI_Status status;
    int pos;

    MPI_Datatype type_ox, type_oy, type_oz;
    MPI_Datatype type_ia, type_ib, type_ic;

    MPI_Init( &argc, &argv );


    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if ((numtasks != 1) && (me!=0)) { 
#ifdef DB_TALK
      printf("Testcase %s uses one task, extraneous task #%d exited.\n",argv[0],me);
#endif
      MPI_Finalize(); 
      exit();
    }

    MPI_Type_contiguous(4,MPI_DOUBLE_INT,&type_ia);
    MPI_Type_commit(&type_ia);
  
    for (i=1;i<256;i++) omessage[i] = i;

    MPI_Send(omessage, 33, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(imessage, 3, type_ia, 0, 0, MPI_COMM_WORLD, &status);

    MPI_Get_elements(&status, type_ia, &count);
    if (count!=MPI_UNDEFINED) {
      printf("ERROR -1- MPI_Get_elements should return MPI_UNDEFINED, not %d.\n",count);
      error++;
    }

    MPI_Get_count(&status, type_ia, &count);
    if (count!=MPI_UNDEFINED) {
      printf("ERROR -1- MPI_Get_count should return MPI_UNDEFINED, not %d.\n",count);
      error++;
    }

    MPI_Send(omessage, 56, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(imessage, 3, type_ia, 0, 0, MPI_COMM_WORLD, &status);

    MPI_Get_elements(&status, type_ia, &count);
    if (count!=9) {
      printf("ERROR -2- MPI_Get_elements should return 9, not %d.\n",count);
      error++;
    }

    MPI_Get_count(&status, type_ia, &count);
    if (count!=MPI_UNDEFINED) {
      printf("ERROR -2- MPI_Get_count should return MPI_UNDEFINED, not %d.\n",count);
      error++;
    }

    MPI_Send(omessage, 60, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(imessage, 3, type_ia, 0, 0, MPI_COMM_WORLD, &status);

    MPI_Get_elements(&status, type_ia, &count);
    if (count!=10) {
      printf("ERROR -3- MPI_Get_elements should return 10, not %d.\n",count);
      error++;
    }

    MPI_Get_count(&status, type_ia, &count);
    if (count!=MPI_UNDEFINED) {
      printf("ERROR -3- MPI_Get_count should return MPI_UNDEFINED, not %d.\n",count);
      error++;
    }

    MPI_Send(omessage, 96, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(imessage, 3, type_ia, 0, 0, MPI_COMM_WORLD, &status);

    MPI_Get_elements(&status, type_ia, &count);
    if (count!=16) {
      printf("ERROR -4- MPI_Get_elements should return 16, not %d.\n",count);
      error++;
    }

    MPI_Get_count(&status, type_ia, &count);
    if (count!=2) {
      printf("ERROR -4- MPI_Get_count should return 2, not %d.\n",count);
      error++;
    }

    if (error)
      printf("MPI_Get_count/MPI_Get_elements test had %d errors.\n",error);
    else
      printf("MPI_Get_count/MPI_Get_elements tests passed.\n");

    MPI_Finalize(); 
}
