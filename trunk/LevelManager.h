#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "Block.h"
#include "Manager.h"
#include "ConsoleListener.h"

/*
	Level represents a logical brother to the level written 
	on the disk. It gets loaded from a file. Then it gets in 
	charge of drawing blocks, water, lava, ceiling in FPS mode,
	terrain effects...
*/

namespace game_utils
{
	namespace managers
	{
		class CLevelManager: public CManager, public control::CConsoleListener
		{
		public:
			CLevelManager();
			~CLevelManager();

			// from CManager
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			// from CConsoleListener
			virtual std::string onAction(std::string keyword, std::string params);

			/*
				Checks is all necessary level files are present.
			*/
			bool levelExists(std::string levelFileName);

			/*
				Returns block if x&y are in normal bounds else NULL.
			*/
			game_objects::CBlock *getBlock(GLint x, GLint y);
			game_objects::CBlock *getBlockOld(GLint x, GLint y);
			game_objects::CBlock *getBlock(cml::vector2i pos);

			GLint getBlockType(GLint x, GLint y);
			GLint getBlockTypeOld(GLint x, GLint y); // for being able to use old code

			bool isBlockTypeNear(GLint blockType, GLint x, GLint y, bool diagonal, GLubyte owner, std::vector<game_objects::CBlock*> *blocks = NULL);
			bool isBlockTypeNear(GLint blockType, cml::vector2i logicalPos, bool diagonal, GLubyte owner, std::vector<game_objects::CBlock*> *blocks = NULL);
			game_objects::CBlock *getUnclaimedBlock(GLubyte owner);
			std::map<game_objects::CBlock*,game_objects::CBlock*> *getUnclaimedBlocksList();
			GLvoid addUnclaimedBlock(game_objects::CBlock *block);
			GLvoid removeUnclaimedBlock(game_objects::CBlock *block);

			// returns true if this block is full type, 
			bool isFullBlock(game_objects::CBlock *block);

			/*
				Some utility methods.
			*/

			/*
				Returns true if block on targetX, targetY is same type and owner as sourceBlock.
			*/
			bool isNotSameTypeAndOwnerAndNotRockOrEarth(GLint targetX, GLint targetY, game_objects::CBlock *sourceBlock);
			bool isSameTypeAndOwner(GLint targetX, GLint targetY, game_objects::CBlock *sourceBlock);
			bool isSameOwner(GLint targetX, GLint targetY, game_objects::CBlock *sourceBlock);
			bool isSameType(GLint targetX, GLint targetY, game_objects::CBlock *sourceBlock);

			/*
				Calculates ceiling hieght fo a specific block. Must be called: 

				1. all blocks init
				2. all block ceiling height
				3. all blocks finalize
			*/
			GLvoid calculateBlockCeilingHeight(game_objects::CBlock *block);

		private:

			game_objects::CBlock *levelMap[85][85];

			/*
				Most important info about level is stored in:
					- OWN tells ownership of the map blocks
					- SLB what type are the map blocks
					- TNG items on the map
			*/
			bool loadSLB(std::string fileName);
			bool loadOWN(std::string fileName);		
			bool loadTNG(std::string fileName);

			/*
				A list of playable levels stored in data\resources\levels.conf.
			*/
			std::vector<std::string> levelFileNames;

			/*
				The block lists used for imps
			*/
			std::map<game_objects::CBlock*,game_objects::CBlock*> unclaimedBlocksList;

			/*
				A currently playing level;
			*/
			GLint currentLevel;
			std::string levelFileName;
			
		};
	};
};

#endif // LEVEL_MANAGER_H