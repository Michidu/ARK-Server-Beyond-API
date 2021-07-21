#include "Commands.h"

#include "IBaseApi.h"

namespace ArkApi
{
	void Commands::AddChatCommand(const FString& command,
		const std::function<void(AShooterPlayerController*, FString*, EChatSendMode::Type)>&
		callback)
	{
		chat_commands_.push_back(std::make_shared<ChatCommand>(command, callback));
	}

	void Commands::AddConsoleCommand(const FString& command,
		const std::function<void(APlayerController*, FString*, bool)>& callback)
	{
		console_commands_.push_back(std::make_shared<ConsoleCommand>(command, callback));
	}

	void Commands::AddRconCommand(const FString& command,
		const std::function<void(RCONClientConnection*, RCONPacket*, UWorld*)>& callback)
	{
		rcon_commands_.push_back(std::make_shared<RconCommand>(command, callback));
	}

	void Commands::AddOnTickCallback(const FString& id, const std::function<void(float)>& callback)
	{
		on_tick_callbacks_.push_back(std::make_shared<OnTickCallback>(id, callback));
	}

	void Commands::AddOnTimerCallback(const FString& id, const std::function<void()>& callback)
	{
		on_timer_callbacks_.push_back(std::make_shared<OnTimerCallback>(id, callback));
	}

	void Commands::AddOnChatMessageCallback(const FString& id,
		const std::function<bool(AShooterPlayerController*, FString*,
			EChatSendMode::Type, bool, bool)>& callback)
	{
		on_chat_message_callbacks_.push_back(std::make_shared<OnChatMessageCallback>(id, callback));
	}

	bool Commands::RemoveChatCommand(const FString& command)
	{
		return RemoveCommand<ChatCommand>(command, chat_commands_);
	}

	bool Commands::RemoveConsoleCommand(const FString& command)
	{
		return RemoveCommand<ConsoleCommand>(command, console_commands_);
	}

	bool Commands::RemoveRconCommand(const FString& command)
	{
		return RemoveCommand<RconCommand>(command, rcon_commands_);
	}

	bool Commands::RemoveOnTickCallback(const FString& id)
	{
		return RemoveCommand<OnTickCallback>(id, on_tick_callbacks_);
	}

	bool Commands::RemoveOnTimerCallback(const FString& id)
	{
		return RemoveCommand<OnTimerCallback>(id, on_timer_callbacks_);
	}

	bool Commands::RemoveOnChatMessageCallback(const FString& id)
	{
		return RemoveCommand<OnChatMessageCallback>(id, on_chat_message_callbacks_);
	}

	bool Commands::CheckChatCommands(AShooterPlayerController* shooter_player_controller, FString* message,
		EChatSendMode::Type mode)
	{
		return CheckCommands<ChatCommand>(*message, chat_commands_, shooter_player_controller, message, mode);
	}

	bool Commands::CheckConsoleCommands(APlayerController* a_player_controller, FString* cmd, bool write_to_log)
	{
		return CheckCommands<ConsoleCommand>(*cmd, console_commands_, a_player_controller, cmd, write_to_log);
	}

	bool Commands::CheckRconCommands(RCONClientConnection* rcon_client_connection, RCONPacket* rcon_packet,
		UWorld* u_world)
	{
		return CheckCommands<RconCommand>(rcon_packet->Body, rcon_commands_, rcon_client_connection, rcon_packet,
			u_world);
	}

	void Commands::CheckOnTickCallbacks(float delta_seconds)
	{
		for (const auto& data : on_tick_callbacks_)
		{
			if (data)
			{
				data->callback(delta_seconds);
			}
		}
	}

	void Commands::CheckOnTimerCallbacks()
	{
		for (const auto& data : on_timer_callbacks_)
		{
			if (data)
			{
				data->callback();
			}
		}
	}

	bool Commands::CheckOnChatMessageCallbacks(
		AShooterPlayerController* player_controller,
		FString* message,
		EChatSendMode::Type mode,
		bool spam_check,
		bool command_executed)
	{
		bool prevent_default = false;
		for (const auto& data : on_chat_message_callbacks_)
		{
			prevent_default |= data->callback(player_controller, message, mode, spam_check, command_executed);
		}

		return prevent_default;
	}

	// Free function
	ICommands& GetCommands()
	{
		return *API::game_api->GetCommands();
	}
} // namespace ArkApi