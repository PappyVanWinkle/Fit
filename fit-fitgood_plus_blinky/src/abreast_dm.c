/****************************************************************************\
**
** abreast_dm.c
**
** Defines functionality for implementation for Abreast DM hash algorithm
**
** Copyright (C) 2016, SafeNet, Inc. All rights reserved.
**
\****************************************************************************/

/* Required Includes ********************************************************/

#include "fit_aes.h"
#include "fit_debug.h"
#include "abreast_dm.h"
#include "dm_hash.h"

/* Global Data **************************************************************/
uint8_t aes256key[32] = {0};

/* Functions ****************************************************************/

/**
 *
 * AES256_AbreastDmHash_Init
 *
 * This function will initialize hash data to default initial value.
 *
 * @param hash --> Pointer to hash data to initialize
 *
 */
void AES256_AbreastDmHash_Init(uint8_t* hash)
{
   // Start Hash with 0xFF
   fit_memset(hash,0xff,32);
}

/**
 *
 * AES_KM_Load256
 *
 * This function will update the aes key (AES algorithm) used in encryption of
 * license data.
 *
 * @param key --> Pointer to aes key data that need to be updated.
 *
 */
void AES_KM_Load256(uint8_t *key)
{
    fit_memcpy(aes256key, key, 32);
}

/**
 *
 * AES_ECB_Encrypt
 *
 * This function will encrypt the data passed in based on global aes key.
 *
 * @param in --> Pointer to data that needs to be encrypted.
 * @param blk_num --> block number in case this function is called message greater
 *                    than 16 bytes.
 *
 */
int AES_ECB_Encrypt(uint8_t *in, uint16_t blk_num)
{
    uint8_t skey[240]   = {{0}};
    uint8_t aes_state[4][4]         = {{0}};
    fit_status_t status = FIT_STATUS_OK;
    aes_state_t aes = {0};
    uint8_t out[16] = {0};

    // Initialize the aes context
    status = aes_setup(&aes,aes256key, AES_256_KEY_LENGTH, skey);
    if (status != 0)
    {
        DBG(FIT_TRACE_ERROR, "failed to initialize aes setup error =%d\n", status);
        return status;
    }

    fit_memset((uint8_t*)aes_state, 0, sizeof(aes_state));
    aes_encrypt(&aes, in, out, skey, (uint8_t*)aes_state);
    fit_memcpy(in, out, 16);

    return FIT_STATUS_OK;
}

/**
 *
 * AES256_AbreastDmHash_Update
 *
 * This function will update the hash of the license data.
 *
 * @param pDataIn --> Buffer to hold data
 * @param NbOfBlocks --> NbOfBlocks Number of data block
 * @param Hash <--> Hash Buffer to hold thye hash value
 *
 */
void AES256_AbreastDmHash_Update(uint8_t* pDataIn, uint16_t NbOfBlocks, uint8_t* Hash)
{
   uint8_t  i;
   uint8_t  tempbuf[16];
   uint8_t* pMsg = pDataIn;
   uint8_t  key[32];
   uint8_t* pHashG = Hash;
   uint8_t* pHashH = Hash + 16;
    
   while(NbOfBlocks--)
   {
      // Gi = Gi-1 XOR AES(Gi-1 || Hi-1Mi)
      fit_memcpy(key,     pHashH, 16); 
      fit_memcpy(key+16,  pMsg,   16);
      AES_KM_Load256(key);

      memcpy(tempbuf, pHashG, 16);
      AES_ECB_Encrypt(tempbuf, 1);
      for(i=0;i<16;i++)
      {
         pHashG[i] ^= tempbuf[i];
      }
            
      // Hi = Hi-1 XOR AES(~ Hi-1 || Mi Gi-1) 
      memcpy(key,     pMsg,   16); 
      memcpy(key+16,  pHashG, 16); 
      AES_KM_Load256(key);

      memcpy(tempbuf, pHashH, 16); 
      for(i=0; i<16; i++)
      {
         tempbuf[i] ^= 0xFF;
      }
      AES_ECB_Encrypt(tempbuf, 1);
      for(i=0;i<16;i++)
      {
         pHashH[i] ^= tempbuf[i];
      }

      // Next block
      pMsg  += 16;
   }
}

/**
 *
 * AES256_AbreastDmHash_UpdateBlk
 *
 * This function will update the hash of the license data (for one block of data)
 *
 * @param pDataIn --> Buffer to hold data
 * @param Hash <--> Hash Buffer to hold thye hash value
 *
 */
void AES256_AbreastDmHash_UpdateBlk(uint8_t* pDataIn, uint8_t* Hash)
{
   uint8_t  i;
   uint8_t  tempbuf[16] = {0};
   uint8_t* pMsg = pDataIn;
   uint8_t  key[32] = {0};
   uint8_t* pHashG = Hash;
   uint8_t* pHashH = Hash + 16;

   // Gi = Gi-1 XOR AES(Gi-1 || Hi-1Mi)
   memcpy(key,     pHashH, 16); 
   memcpy(key+16,  pMsg,   16); 
   AES_KM_Load256(key);

   memcpy(tempbuf, pHashG, 16);
   AES_ECB_Encrypt(tempbuf, 1);
   for(i=0;i<16;i++)
   {
      pHashG[i] ^= tempbuf[i];
   }

   // Hi = Hi-1 XOR AES(~ Hi-1 || Mi Gi-1) 
   memcpy(key,     pMsg,   16); 
   memcpy(key+16,  pHashG, 16); 
   AES_KM_Load256(key);

   memcpy(tempbuf, pHashH, 16); 
   for(i=0; i<16; i++)
   {
      tempbuf[i] ^= 0xFF;
   }
   AES_ECB_Encrypt(tempbuf, 1);
   for(i=0;i<16;i++)
   {
      pHashH[i] ^= tempbuf[i];
   }
}

/**
 *
 * AES256_AbreastDmHash_Finalize
 *
 * This function will perform final update on hash of the license data
 *
 * @param Hash <--> Hash Buffer to hold thye hash value
 *
 */
void AES256_AbreastDmHash_Finalize(uint8_t* Hash)
{
   uint8_t i;
   uint8_t tempbuf[16] = {0};
   uint8_t tempkey[32] = {0};
    
   memcpy(tempkey, Hash, 32);
   AES_KM_Load256(tempkey);
   // Hash[0-15]
   memcpy(tempbuf, Hash, 16);
   AES_ECB_Encrypt(tempbuf, 1);
   for(i =0; i< 16; i++)
   {
      Hash[i] ^= tempbuf[i];
   }
   // Hash[16-32]
   memcpy(tempbuf, Hash+16, 16);
   AES_ECB_Encrypt(tempbuf, 1);
   for(i =0; i< 16; i++)
   {
      Hash[i+16] ^= tempbuf[i];
   }
      
   return;
}

/**
 *
 * fit_get_AbreastDM_Hash
 *
 * This function will get the abreast dm hash of the data passed in.
 *
 * @param   msg --> Pointer to data passed in for which hash needs to be calculated.
 * @param   Hash <--> Hash Buffer to hold thye hash value
 *
 */
fit_status_t fit_get_AbreastDM_Hash(fit_pointer_t *msg, uint8_t * hash)
{
    uint16_t cntr           = 0;
    uint8_t tempmsg[32]     = {0};
    uint16_t msglen         = 0;
    fit_pointer_t fitptr    = {0};
    // Initialize the read pointer.
    fitptr.read_byte = msg->read_byte;

    // Initialize hash value;
    AES256_AbreastDmHash_Init(hash);

    // Break data in blocks (16 bytes each) and hash the data.
    for (cntr = 0; cntr < msg->length; cntr+=16)
    {
        if ((cntr+16) < msg->length)
            AES256_AbreastDmHash_UpdateBlk((msg->data)+cntr, hash);
    }
    cntr -= 16;

    fitptr.data = msg->data+cntr;
    fitptr.length = msg->length-cntr;
    msglen = fitptr.length;
    fitptr_memcpy(tempmsg, &fitptr);
    

    fit_dm_hash_init(tempmsg, &msglen, msg->length);
    for (cntr = 0; cntr < msglen; cntr+=16)
    {
        AES256_AbreastDmHash_UpdateBlk(tempmsg+cntr, hash);
    }

    AES256_AbreastDmHash_Finalize(hash);

    return FIT_STATUS_OK;
}
