#include <includes_pch.h>

enum State {
	VIDEO_SEND = 1,
	VIDEO_NAME,
	VIDEO_DESCRIPTION,
	VIDEO_HASHTAGS,
	VIDEO_SEND_FINISH,
};

std::string ReadToken()
{
	// Temp workaround because executable path is deep in 'out/...' folder
	std::ifstream ifs("../../../../token.txt");
	if (!ifs.is_open())
		assert(true, "'token.txt' isn't open");

	std::string result;
	std::getline(ifs, result);
	ifs.close();
	return result;
}

int main()
{
	std::string token = ReadToken();
	printf("Token: %s\n", token.c_str());
	TgBot::Bot bot(token);

	TgBot::InlineQueryResult::Ptr ex1(new TgBot::InlineQueryResult);
	TgBot::InlineQueryResult::Ptr ex2(new TgBot::InlineQueryResultVideo);
	TgBot::InlineQueryResultVideo::Ptr ex3(new TgBot::InlineQueryResultVideo);
	std::cout << std::format("type is: '{}'\n", ex1->type);
	std::cout << std::format("type is: '{}'\n", ex2->type);
	std::cout << std::format("type is: '{}'\n", ex3->type);

	bot.getEvents().onInlineQuery([&bot](TgBot::InlineQuery::Ptr query) {
		auto& text = query->query;
		std::cout << std::format("Current InlineQuery: '{}'\n", text);
		bot.getApi().answerInlineQuery(query->id, {}, 60, false, "");

		});

	TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
	std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
	TgBot::InlineKeyboardButton::Ptr checkButton(new TgBot::InlineKeyboardButton);
	checkButton->text = "check";
	checkButton->callbackData = "check";
	row0.push_back(checkButton);
	keyboard->inlineKeyboard.push_back(row0);

	bot.getEvents().onCommand("start", [&bot, &keyboard](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(message->chat->id, "Hi!", false, 0, keyboard);

		});
	bot.getEvents().onCommand("check", [&bot, &keyboard](TgBot::Message::Ptr message) {
		std::string response = "ok";
		bot.getApi().sendMessage(message->chat->id, response, false, 0, keyboard, "Markdown");
		});
	bot.getEvents().onCallbackQuery([&bot, &keyboard](TgBot::CallbackQuery::Ptr query) {
		if (StringTools::startsWith(query->data, "check")) {
			std::string response = "ok";
			//bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
			bot.getApi().answerCallbackQuery(query->id, "Button pressed");
		}
		});

	typedef struct {
		State state;
		std::string vidId;
	} VideoInfo;

	std::map<uint64_t, VideoInfo> map_states;

	bot.getEvents().onAnyMessage([&bot, &map_states](TgBot::Message::Ptr msg) {

		auto idChat = msg->chat->id;
		auto idUser = msg->from->id;
		auto video = msg->video;
		auto& text = msg->text;


		if (map_states.contains(idUser))
		{
			if (video)
			{
				bot.getApi().sendMessage(msg->chat->id, "Video has been sent", false, 0);
			}

			std::string response; response.reserve(512);
			switch (map_states[idUser].state)
			{
			case VIDEO_SEND:
				if (text == "#")
				{
					map_states.erase(idUser);
					response = U8("You successfully canceled video uploading.");
					bot.getApi().sendMessage(idChat, response);
					return;
				}
				if (video)
				{

					std::cout << std::format("file id is: '{}'\n", video->fileId);
					auto file = bot.getApi().getFile(video->fileId);
					file->filePath;

					std::cout << std::format("file path is: '{}'\n", file->filePath);
					auto newFile = bot.getApi().downloadFile(file->filePath);

					std::ofstream fout("some_sent_video.mp4");
					fout << newFile;
					fout.close();

					map_states[idUser].state = State::VIDEO_NAME;
					map_states[idUser].vidId = video->fileId;
					[[fallthrough]];
				}
				else
				{
					response = U8("Upload video or write '#' to exit the process");
					bot.getApi().sendMessage(idChat, response);
					break;
				}
			case VIDEO_NAME:
				if (text == "#")
				{
					map_states.erase(idUser);
					response = U8("You successfully canceled video uploading.");
					bot.getApi().sendMessage(idChat, response);
					return;
				}

				response = U8("Enter video name or write '#' to exit the process");
				map_states[idUser].state = State::VIDEO_DESCRIPTION;
				bot.getApi().sendMessage(idChat, response);
				break;
			case VIDEO_DESCRIPTION:
				if (text == "#")
				{
					map_states.erase(idUser);
					response = U8("You successfully canceled video uploading.");
					bot.getApi().sendMessage(idChat, response);
					return;
				}

				response = U8("Enter video description or write '$' to skip step and '#' to exit the process");
				map_states[idUser].state = State::VIDEO_HASHTAGS;
				bot.getApi().sendMessage(idChat, response);
				break;
			case VIDEO_HASHTAGS:
				if (text == "#")
				{
					map_states.erase(idUser);
					response = U8("You successfully canceled video uploading.");
					bot.getApi().sendMessage(idChat, response);
					return;
				}

				response = U8("Enter video hashtags or write '$' to skip step and '#' to exit the process");
				map_states[idUser].state = State::VIDEO_SEND_FINISH;
				bot.getApi().sendMessage(idChat, response);
				break;
			case VIDEO_SEND_FINISH:
				response = U8("You successfully uploaded a video");
				// #TODO save video info
				map_states.erase(idUser);
				bot.getApi().sendMessage(idChat, response);
				break;
			default:
				response = U8("To upload video use '/add_video' command");
				bot.getApi().sendMessage(idChat, response);
				break;
			}

			return;
		}

		if (msg->text.starts_with("/add_video"))
		{
			map_states.insert({ (uint64_t)idUser, { State::VIDEO_SEND, ""} });
			std::string respone = U8("Send video or write '#' to exit the process");

			bot.getApi().sendMessage(idChat, respone);
			return;
		}
		});

	signal(SIGINT, [](int s) {
		printf("SIGINT got\n");
		exit(0);
		});

	try {
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		bot.getApi().deleteWebhook();

		TgBot::TgLongPoll longPoll(bot);
		while (true) {
			printf("Long poll started\n");
			longPoll.start();
		}
	}
	catch (std::exception& e) {
		printf("error: %s\n", e.what());
	}

	return 0;
}
