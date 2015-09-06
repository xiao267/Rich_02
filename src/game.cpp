#include "game.h"
//���ó�ʼ�ʽ�
void Game::setDefalutMoney()
{
	vector<Player>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
	{
		it->setPlayerAssets(defaultMoney);
	}


	/*
	/////////////////////////
	vector<Player>::iterator ait;
	for (ait = players.begin(); ait != players.end(); ++ait)
	{
		cout << endl;
		cout << ait->getPlayerAssets() << " " << ait->getPlayerId() << endl;

	}*/
}

/*
*������initPlayer()
*����˵����
*���ܣ���ʼ����ң�������ҵ��ʽ�λ��
*����ֵ����
*���ߣ�
*/
void Game::initPlayer()
{
	char input[MAX_PLAYER_NUM];
	choicePlayer(input);
	
	for (int i = 0; i < playerNum; ++i)
	{
		int num = charToInt(input[i]);
		addPlayer(Player(STAY_PLAYERS[num - 1], i + 1, PLAYER_SIGN[num - 1]));
	}
	setDefalutMoney();
}



/*
*������initGame()
*����˵����
*���ܣ���ʼ����ͼ�����
*����ֵ����
*���ߣ�
*/
void Game::initGame()
{
	map->initMap();
	map->printMap();
	cout << "\n\n��Ϸ��ʼ, ��ѡ����ҽ��г�ʼ����Һ��ʽ�" << endl;
	cout << "��ѡ��2~4λ���ظ���ң�����������ұ�ź�س����ɣ��磺123��\n(1.Ǯ����; 2.������; 3.��С��; 4.�𱴱�):";
	initPlayer();
}

/*
*������destroyPlayer()
*����˵����
*���ܣ����ٵ�ͼ
*����ֵ����
*���ߣ�
*/
void Game::destroyGame() {}

/*
*������quitGame()
*����˵����
*���ܣ���ֹ��Ϸ
*����ֵ����
*���ߣ�
*/
void Game::quitGame() {}

/*
*������addPlayer()
*����˵����
*���ܣ��������
*����ֵ����
*���ߣ�
*/
void Game::addPlayer(Player player)
{
	players.push_back(player);
}

/*
*������deletePlayer()
*����˵����
*���ܣ�ɾ�����
*����ֵ����
*���ߣ�
*/
void Game::deletePlayer(vector<Player>::iterator it)
{
	
	players.erase(it);
}

//��Ϸ����,��Ҫ�������һ�θ��ֲ���
void Game::goGameBody()
{
	//��ȡ��ǰ���
	vector<Player>::iterator it;
	vector<Player>::iterator preIt;
	it = players.begin();
	for (;;)
	{
		//ˢ�µ�ͼ
		flushMap();

		//�����˳�
		bool doAction = false;
		//���ִ��roll
		int roll_once = 0;
		//����
		char tempCmd[MAX_COMMAND_SIZE];
		
		//��ʾ�û�����
		cout << it->getPlayerName() << ">";

		for (;;)
		{
			//ִ������
			getCommandInput(tempCmd);
			//���ǿ���˳�
			if (command->equalCommand(tempCmd, "quit")) return;
			if ((command->equalCommand(tempCmd, "finish")) && (roll_once == 1))
			{
				break;
			}
			if (command->equalCommand(tempCmd, "roll"))
			{
				roll_once = 1;
				int step = command->roll();
				cout << "��ǰ�н�" << step << "����" << endl;

				PlayerAction::levPlace(it, map);
				PlayerAction::resetPlayerPos(it, map, step);

				//ˢ�µ�ͼ
				flushMap();
				//��ʾ�û�����
				cout << it->getPlayerName() << ">";
				//ִ�в�ͬλ�ò���
				doAction = PlayerAction::stayInDiffPlace(it, map, it->getPlayerPos());
				it++;
				break;

			}
			else if (command->equalCommand(tempCmd, "help"))
			{
				command->help();
			}
			else if (command->equalCommand(tempCmd, "query"))
			{
				command->query(it);
			}


			//exit������
			else if (command->equalCommand(tempCmd, "exit"))
			{
				//command->exit(it, map);
				
				deletePlayer(it);
				it++;
				break;
			}
			else if (command->equalCommand(tempCmd, "sell"))
			{
				if (it->getOwnPosNum() != 0)
				{
					command->sell(it, map);
				}
				else
				{
					cout << "������û�еز�" << endl;
				}
				
				
			}
			else
			{
				cout << "û����ѡ�������,������help�鿴����" << endl;
			}

		}
		if (!doAction && PlayerAction::isNoAssets(it))
		{
			PlayerAction::queryAssets(it);
			cout << "�㵱ǰ�ʲ��Ѿ��������汾��Ϸ�� ��Ϸ���������˳�" << endl;
			command->exit(it, map);
			deletePlayer(it);

		}
		
		if (it == players.end())
		{
			it = players.begin();
		}
		if (players.size() == 1)
		{
			cout << "��Ϸ����" << endl;
			return;
		}
		//ˢ����Ļ
		system("cls");

	}
}

//ִ����Ϸ���ٴη�װ��Ϸ
void Game::goGame()
{
	char isPlay = 'y';
	while(isPlay == 'y') 
	{
	initGame();
	goGameBody();
	cout << "�Ƿ����¿�ʼ��Ϸ��y or n";
	cin >> isPlay;
	}
}

//ѡ�����
void Game::choicePlayer(char *input)
{
	do
	{
		getPlayerInput(input);
		if (isInputPlayer(input) || (!isExistPlayer(input)))
		{
			cout << "��������ע�������ʽ����Ҫ�ظ�ѡ�����" << endl;
			continue;
		}
		playerNum = strlen(input);
	} while (playerNum < MIN_PLAYER_NUM || playerNum > MAX_PLAYER_NUM);


}

//�������Ƿ����
bool Game::isExistPlayer(char *input)
{
	for (int i = 0; i < strlen(input); i++)
	{
		if (input[i] > '4' || input[i] < '1')
		{
			return false;
		}
	}
	return true;
}

//�����������Ƿ��ظ�
bool Game::isInputPlayer(char *input)
{
	for (int i = 0; i<strlen(input); i++)
	{
		for (int j = i + 1; j<strlen(input); j++)
		{
			if (input[i] == input[j])
			{
				return true;
			}
		}
	}
	return false;
}


//�������
void Game::getPlayerInput(char *input)
{
	getInput(input, MAX_PLAYER_NUM);
}

//��������
void Game::getCommandInput(char *input)
{
	getInput(input, MAX_COMMAND_SIZE);
}

void Game::flushMap()
{
	system("cls");
	map->printMap();
	cout << "����������,����finish�˳����������help��ð�����Ϣ����" << endl;
}