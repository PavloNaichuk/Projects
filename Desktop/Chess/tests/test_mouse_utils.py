from mouse_utils import (
    get_board_square,
    has_pawn_reached_last_rank,
    is_board_click,
    is_move_allowed,
    is_pawn,
)


def test_is_board_click_returns_true_inside_board():
    assert is_board_click((639, 100), board_width=640) is True


def test_is_board_click_returns_false_outside_board():
    assert is_board_click((640, 100), board_width=640) is False
    assert is_board_click((700, 100), board_width=640) is False


def test_get_board_square_from_mouse_position():
    assert get_board_square((0, 0), square_size=80) == (0, 0)
    assert get_board_square((79, 79), square_size=80) == (0, 0)
    assert get_board_square((80, 160), square_size=80) == (2, 1)


def test_is_move_allowed_for_local_mode():
    assert is_move_allowed(
        mode="local",
        vs_bot=False,
        game_turn="b",
        net=None,
        local_turn=None,
    ) is True


def test_is_move_allowed_for_bot_only_when_white_to_move():
    assert is_move_allowed(
        mode="bot",
        vs_bot=True,
        game_turn="w",
        net=None,
        local_turn=None,
    ) is True

    assert is_move_allowed(
        mode="bot",
        vs_bot=True,
        game_turn="b",
        net=None,
        local_turn=None,
    ) is False


def test_is_move_allowed_for_network_only_on_local_turn():
    net = object()

    assert is_move_allowed(
        mode="net_client",
        vs_bot=False,
        game_turn="b",
        net=net,
        local_turn="b",
    ) is True

    assert is_move_allowed(
        mode="net_client",
        vs_bot=False,
        game_turn="w",
        net=net,
        local_turn="b",
    ) is False


def test_is_pawn():
    assert is_pawn("wP") is True
    assert is_pawn("bP") is True
    assert is_pawn("wN") is False
    assert is_pawn(None) is False


def test_has_pawn_reached_last_rank():
    assert has_pawn_reached_last_rank("wP", 0) is True
    assert has_pawn_reached_last_rank("wP", 1) is False
    assert has_pawn_reached_last_rank("bP", 7) is True
    assert has_pawn_reached_last_rank("bP", 6) is False
    assert has_pawn_reached_last_rank("wN", 0) is False