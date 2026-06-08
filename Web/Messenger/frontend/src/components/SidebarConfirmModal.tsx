type SidebarConfirmModalProps = {
  title: string;
  message: string;
  confirmLabel: string;
  loadingLabel: string;
  isLoading: boolean;
  isDanger?: boolean;
  onCancel: () => void;
  onConfirm: () => void | Promise<void>;
};

function SidebarConfirmModal({
  title,
  message,
  confirmLabel,
  loadingLabel,
  isLoading,
  isDanger = false,
  onCancel,
  onConfirm,
}: SidebarConfirmModalProps) {
  return (
    <div className="modal-backdrop">
      <div className="confirm-modal" role="dialog" aria-modal="true">
        <h3>{title}</h3>

        <p>{message}</p>

        <div className="confirm-modal-actions">
          <button
            type="button"
            className="confirm-modal-cancel"
            onClick={onCancel}
            disabled={isLoading}
          >
            Cancel
          </button>

          <button
            type="button"
            className={
              isDanger ? "confirm-modal-delete danger" : "confirm-modal-delete"
            }
            onClick={onConfirm}
            disabled={isLoading}
          >
            {isLoading ? loadingLabel : confirmLabel}
          </button>
        </div>
      </div>
    </div>
  );
}

export default SidebarConfirmModal;
